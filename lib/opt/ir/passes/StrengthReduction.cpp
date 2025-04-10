#include "mini-llvm/opt/ir/passes/StrengthReduction.h"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/And.h"
#include "mini-llvm/ir/Instruction/ASHR.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/SDiv.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Instruction/SRem.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/UDiv.h"
#include "mini-llvm/ir/Instruction/URem.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

// Non-Adjacent Form (NAF)
// https://en.wikipedia.org/wiki/Non-adjacent_form

std::vector<int> computeNAF(uint64_t n) {
    std::vector<int> naf;
    while (n) {
        if (n % 2 == 1) {
            int d = 2 - static_cast<int>(n % 4);
            naf.push_back(d);
            n -= d;
        } else {
            naf.push_back(0);
        }
        n /= 2;
    }
    return naf;
}

std::vector<std::shared_ptr<Instruction>> replaceMulM1(Value &lhs) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    replaced.emplace_back(std::make_shared<Sub>(lhs.type()->constant(0), share(lhs)));

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceMulGeneral(Value &lhs, uint64_t rhs) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    std::vector<std::pair<std::shared_ptr<Value>, int>> terms;
    std::vector<int> naf = computeNAF(rhs);
    if (naf[0] != 0) {
        terms.emplace_back(share(lhs), naf[0]);
    }
    for (size_t i = 1; i < naf.size(); ++i) {
        if (naf[i] != 0) {
            std::shared_ptr<Instruction> t = std::make_shared<SHL>(share(lhs), lhs.type()->constant(i));
            replaced.emplace_back(t);
            terms.emplace_back(t, naf[i]);
        }
    }
    if (terms[0].second < 0) {
        terms.emplace(terms.begin(), lhs.type()->constant(0), 1);
    }
    if (terms.size() >= 2) {
        if (terms[1].second > 0) {
            replaced.emplace_back(std::make_shared<Add>(terms[0].first, terms[1].first));
        } else {
            replaced.emplace_back(std::make_shared<Sub>(terms[0].first, terms[1].first));
        }
        for (size_t i = 2; i < terms.size(); ++i) {
            if (terms[i].second > 0) {
                replaced.emplace_back(std::make_shared<Add>(replaced.back(), terms[i].first));
            } else {
                replaced.emplace_back(std::make_shared<Sub>(replaced.back(), terms[i].first));
            }
        }
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceMul(Value &lhs, uint64_t rhs) {
    if (rhs == 0 || rhs == 1) {
        return {};
    }

    if (rhs == (static_cast<uint64_t>(-1) >> (64 - lhs.type()->sizeInBits()))) { // -1
        return replaceMulM1(lhs);
    }

    return replaceMulGeneral(lhs, rhs);
}

std::vector<std::shared_ptr<Instruction>> replaceMul(const Mul &I) {
    if (!dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        return {};
    }

    uint64_t rhs = static_cast<const IntegerConstant *>(&*I.rhs())->zeroExtendedValue();

    return replaceMul(*I.lhs(), rhs);
}

// Torbjorn Granlund and Peter L. Montgomery. 1994. Division by invariant integers using multiplication.
// In Proceedings of the ACM SIGPLAN 1994 conference on Programming language design and implementation (PLDI '94).
// Association for Computing Machinery, New York, NY, USA, 61-72. https://doi.org/10.1145/178243.178249

std::vector<std::shared_ptr<Instruction>> replaceUDivLarge(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    std::shared_ptr<Instruction> t1 = std::make_shared<ICmp>(ICmp::Condition::kUGE, share(n), n.type()->constant(d));
    std::shared_ptr<Instruction> t2 = std::make_shared<ZExt>(t1, cast<IntegerType>(n.type()));

    replaced.push_back(t1);
    replaced.push_back(t2);

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceUDivPow2(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    replaced.push_back(std::make_shared<LSHR>(share(n), n.type()->constant(std::countr_zero(d))));

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceUDivGeneral(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->sizeInBits();
    int l = N - std::countl_zero(static_cast<uint32_t>(d - 1));
    uint32_t m = (UINT64_C(1) << (N + l)) / d - (UINT64_C(1) << N) + 1;

    std::shared_ptr<Instruction> t1 = std::make_shared<ZExt>(share(n), cast<IntegerType>(n.type()->promoted()));
    replaced.push_back(t1);

    std::shared_ptr<Instruction> t2;
    if (m == 1) {
        t2 = t1;
    } else {
        t2 = std::make_shared<Mul>(t1, n.type()->promoted()->constant(m));
        replaced.push_back(t2);
    }

    std::shared_ptr<Instruction> t3 = std::make_shared<LSHR>(t2, n.type()->promoted()->constant(N));
    std::shared_ptr<Instruction> t4 = std::make_shared<Add>(t1, t3);
    std::shared_ptr<Instruction> t5 = std::make_shared<LSHR>(t4, n.type()->promoted()->constant(l));
    std::shared_ptr<Instruction> t6 = std::make_shared<Trunc>(t5, cast<IntegerType>(n.type()));

    replaced.push_back(t3);
    replaced.push_back(t4);
    replaced.push_back(t5);
    replaced.push_back(t6);

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceUDiv(Value &n, uint64_t d) {
    if (d == 0 || d == 1) {
        return {};
    }

    if (d >= (UINT64_C(1) << (n.type()->sizeInBits() - 1))) {
        return replaceUDivLarge(n, d);
    }

    if (std::has_single_bit(d)) {
        return replaceUDivPow2(n, d);
    }

    if (!dynamic_cast<const I64 *>(&*n.type())) {
        return replaceUDivGeneral(n, d);
    }

    return {};
}

std::vector<std::shared_ptr<Instruction>> replaceUDiv(const UDiv &I) {
    if (!dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        return {};
    }

    uint64_t d = static_cast<const IntegerConstant *>(&*I.rhs())->zeroExtendedValue();

    return replaceUDiv(*I.lhs(), d);
}

std::vector<std::shared_ptr<Instruction>> replaceSDivM1(Value &n) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    replaced.push_back(std::make_shared<Sub>(n.type()->constant(0), share(n)));

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSDivPow2(Value &n, int64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->sizeInBits();
    int l = std::countr_zero(static_cast<uint64_t>(std::abs(d)));

    std::shared_ptr<Instruction> t1 = std::make_shared<ASHR>(share(n), n.type()->constant(l - 1));
    std::shared_ptr<Instruction> t2 = std::make_shared<LSHR>(t1, n.type()->constant(N - l));
    std::shared_ptr<Instruction> t3 = std::make_shared<Add>(share(n), t2);
    std::shared_ptr<Instruction> t4 = std::make_shared<ASHR>(t3, n.type()->constant(l));

    replaced.push_back(t1);
    replaced.push_back(t2);
    replaced.push_back(t3);
    replaced.push_back(t4);

    if (d < 0) {
        std::shared_ptr<Instruction> t5 = std::make_shared<Sub>(n.type()->constant(0), t4);
        replaced.push_back(t5);
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSDivGeneral(Value &n, int64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->sizeInBits();
    int l = std::max(N - std::countl_zero(static_cast<uint32_t>(std::abs(d) - 1)), 1);
    int32_t m = static_cast<int32_t>(1 + (UINT64_C(1) << (N + l - 1)) / std::abs(d) - (UINT64_C(1) << N));

    std::shared_ptr<Instruction> t1 = std::make_shared<ZExt>(share(n), cast<IntegerType>(n.type()->promoted()));
    replaced.push_back(t1);

    std::shared_ptr<Instruction> t2;
    if (m == 1) {
        t2 = t1;
    } else {
        t2 = std::make_shared<Mul>(t1, n.type()->promoted()->constant(m));
        replaced.push_back(t2);
    }

    std::shared_ptr<Instruction> t3 = std::make_shared<LSHR>(t2, n.type()->promoted()->constant(N));
    std::shared_ptr<Instruction> t4 = std::make_shared<Trunc>(t3, cast<IntegerType>(n.type()));
    std::shared_ptr<Instruction> t5 = std::make_shared<Add>(share(n), t4);
    std::shared_ptr<Instruction> t6 = std::make_shared<ASHR>(t5, n.type()->constant(l - 1));
    std::shared_ptr<Instruction> t7 = std::make_shared<ASHR>(share(n), n.type()->constant(N - 1));
    std::shared_ptr<Instruction> t8 = std::make_shared<Sub>(t6, t7);

    replaced.push_back(t3);
    replaced.push_back(t4);
    replaced.push_back(t5);
    replaced.push_back(t6);
    replaced.push_back(t7);
    replaced.push_back(t8);

    if (d < 0) {
        std::shared_ptr<Instruction> t9 = std::make_shared<Sub>(n.type()->constant(0), t8);
        replaced.push_back(t9);
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSDiv(Value &n, int64_t d) {
    if (d == 0 || d == 1) {
        return {};
    }

    if (d == -1) {
        return replaceSDivM1(n);
    }

    if (std::has_single_bit(static_cast<uint64_t>(std::abs(d)))) {
        return replaceSDivPow2(n, d);
    }

    return replaceSDivGeneral(n, d);
}

std::vector<std::shared_ptr<Instruction>> replaceSDiv(const SDiv &I) {
    if (!dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        return {};
    }

    int64_t d = static_cast<const IntegerConstant *>(&*I.rhs())->signExtendedValue();

    return replaceSDiv(*I.lhs(), d);
}

std::vector<std::shared_ptr<Instruction>> replaceURemPow2(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    replaced.push_back(std::make_shared<And>(share(n), n.type()->constant(d - 1)));

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceURemGeneral(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced = replaceUDiv(n, d);

    if (!replaced.empty()) {
        std::shared_ptr<Instruction> t0 = replaced.back();
        std::shared_ptr<Instruction> t1 = std::make_shared<Mul>(t0, n.type()->constant(d));
        std::shared_ptr<Instruction> t2 = std::make_shared<Sub>(share(n), t1);

        replaced.push_back(t1);
        replaced.push_back(t2);
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceURem(Value &n, uint64_t d) {
    if (d == 0 || d == 1) {
        return {};
    }

    if (std::has_single_bit(d)) {
        return replaceURemPow2(n, d);
    }

    return replaceURemGeneral(n, d);
}

std::vector<std::shared_ptr<Instruction>> replaceURem(const URem &I) {
    if (!dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        return {};
    }

    uint64_t d = static_cast<const IntegerConstant *>(&*I.rhs())->zeroExtendedValue();

    return replaceURem(*I.lhs(), d);
}

std::vector<std::shared_ptr<Instruction>> replaceSRem(Value &n, int64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced = replaceSDiv(n, d);

    if (!replaced.empty()) {
        std::shared_ptr<Instruction> t0 = replaced.back();
        std::shared_ptr<Instruction> t1 = std::make_shared<Mul>(t0, n.type()->constant(d));
        std::shared_ptr<Instruction> t2 = std::make_shared<Sub>(share(n), t1);

        replaced.push_back(t1);
        replaced.push_back(t2);
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSRem(const SRem &I) {
    if (!dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        return {};
    }

    int64_t d = static_cast<const IntegerConstant *>(&*I.rhs())->signExtendedValue();

    return replaceSRem(*I.lhs(), d);
}

void dfs(const DominatorTreeNode *node, bool &changed, size_t mulThreshold, size_t divThreshold, size_t remThreshold) {
    std::vector<const Instruction *> remove;

    for (const Instruction &I : *node->block) {
        std::vector<std::shared_ptr<Instruction>> replaced;

        if (auto *mul = dynamic_cast<const Mul *>(&I)) {
            replaced = replaceMul(*mul);

            if (replaced.size() > mulThreshold) {
                replaced = {};
            }
        } else if (auto *udiv = dynamic_cast<const UDiv *>(&I)) {
            replaced = replaceUDiv(*udiv);

            if (replaced.size() > divThreshold) {
                replaced = {};
            }
        } else if (auto *sdiv = dynamic_cast<const SDiv *>(&I)) {
            replaced = replaceSDiv(*sdiv);

            if (replaced.size() > divThreshold) {
                replaced = {};
            }
        } else if (auto *urem = dynamic_cast<const URem *>(&I)) {
            replaced = replaceURem(*urem);

            if (replaced.size() > remThreshold) {
                replaced = {};
            }
        } else if (auto *srem = dynamic_cast<const SRem *>(&I)) {
            replaced = replaceSRem(*srem);

            if (replaced.size() > remThreshold) {
                replaced = {};
            }
        }

        if (!replaced.empty()) {
            Instruction *result = &*replaced.back();
            for (auto &II : replaced) {
                addToParent(I, std::move(II));
                changed = true;
            }
            changed |= replaceAllUsesWith(I, share(*result));
            remove.push_back(&I);
        }
    }

    for (const Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    for (const DominatorTreeNode *child : node->children) {
        dfs(child, changed, mulThreshold, divThreshold, remThreshold);
    }
}

} // namespace

bool StrengthReduction::runOnFunction(Function &F) {
    bool changed = false;

    bool changed2;
    do {
        changed2 = false;

        for (BasicBlock &B : F) {
            for (Instruction &I : B) {
                if (auto *op = dynamic_cast<BinaryIntegerArithmeticOperator *>(&I)) {
                    if (op->isCommutative()
                            && dynamic_cast<const IntegerConstant *>(&*op->lhs())
                            && !dynamic_cast<const IntegerConstant *>(&*op->rhs())) {
                        std::shared_ptr<Value> lhs = share(*op->lhs()),
                                               rhs = share(*op->rhs());
                        op->lhs().set(std::move(rhs));
                        op->rhs().set(std::move(lhs));
                        changed2 = true;
                    }
                }
            }
        }

        DominatorTreeAnalysis domTree;
        domTree.runOnFunction(F);

        dfs(domTree.node(F.entry()), changed2, mulThreshold_, divThreshold_, remThreshold_);

        changed |= changed2;
    } while (changed2);

    return changed;
}
