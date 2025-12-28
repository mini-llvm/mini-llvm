// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/StrengthReduction.h"

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
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
#include "mini-llvm/ir/Instruction/SExt.h"
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
#include "mini-llvm/utils/SequenceOps.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;
using namespace mini_llvm::sequence_ops;

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

std::vector<std::shared_ptr<Instruction>> replaceMulNeg1(Value &lhs) {
    return std::vector<std::shared_ptr<Instruction>>() + std::make_shared<Sub>(lhs.type()->constant(0), share(lhs));
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
            std::shared_ptr<Instruction> x = std::make_shared<SHL>(share(lhs), lhs.type()->constant(i));
            replaced += x;
            terms.emplace_back(x, naf[i]);
        }
    }
    if (terms[0].second < 0) {
        terms.emplace(terms.begin(), lhs.type()->constant(0), 1);
    }
    if (terms.size() >= 2) {
        if (terms[1].second > 0) {
            replaced += std::make_shared<Add>(terms[0].first, terms[1].first);
        } else {
            replaced += std::make_shared<Sub>(terms[0].first, terms[1].first);
        }
        for (size_t i = 2; i < terms.size(); ++i) {
            if (terms[i].second > 0) {
                replaced += std::make_shared<Add>(replaced.back(), terms[i].first);
            } else {
                replaced += std::make_shared<Sub>(replaced.back(), terms[i].first);
            }
        }
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceMul(Value &lhs, uint64_t rhs) {
    if (rhs == 0 || rhs == 1) {
        return {};
    }

    if (rhs == (static_cast<uint64_t>(-1) >> (64 - lhs.type()->bitSize()))) { // -1
        return replaceMulNeg1(lhs);
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
    std::shared_ptr<Instruction> x1 = std::make_shared<ICmp>(ICmp::Condition::kUGE, share(n), n.type()->constant(d));
    std::shared_ptr<Instruction> x2 = std::make_shared<ZExt>(x1, cast<IntegerType>(n.type()));

    return std::vector<std::shared_ptr<Instruction>>() + x1 + x2;
}

std::vector<std::shared_ptr<Instruction>> replaceUDivPow2(Value &n, uint64_t d) {
    return std::vector<std::shared_ptr<Instruction>>() +
           std::make_shared<LSHR>(share(n), n.type()->constant(std::countr_zero(d)));
}

std::vector<std::shared_ptr<Instruction>> replaceUDivGeneral(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->bitSize();
    int l = N - std::countl_zero(static_cast<uint32_t>(d - 1));
    uint32_t m = (UINT64_C(1) << (N + l)) / d - (UINT64_C(1) << N) + 1;

    std::shared_ptr<Instruction> x1 = std::make_shared<ZExt>(share(n), cast<IntegerType>(n.type()->promoted()));
    replaced += x1;

    std::shared_ptr<Instruction> x2;
    if (m == 1) {
        x2 = x1;
    } else {
        x2 = std::make_shared<Mul>(x1, n.type()->promoted()->constant(m));
        replaced += x2;
    }

    std::shared_ptr<Instruction> x3 = std::make_shared<LSHR>(x2, n.type()->promoted()->constant(N));
    std::shared_ptr<Instruction> x4 = std::make_shared<Add>(x1, x3);
    std::shared_ptr<Instruction> x5 = std::make_shared<LSHR>(x4, n.type()->promoted()->constant(l));
    std::shared_ptr<Instruction> x6 = std::make_shared<Trunc>(x5, cast<IntegerType>(n.type()));

    replaced += x3, x4, x5, x6;

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceUDiv(Value &n, uint64_t d) {
    if (d == 0 || d == 1) {
        return {};
    }

    if (d >= (UINT64_C(1) << (n.type()->bitSize() - 1))) {
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

std::vector<std::shared_ptr<Instruction>> replaceSDivNeg1(Value &n) {
    return std::vector<std::shared_ptr<Instruction>>() + std::make_shared<Sub>(n.type()->constant(0), share(n));
}

std::vector<std::shared_ptr<Instruction>> replaceSDivPow2(Value &n, int64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->bitSize();
    int l = std::countr_zero(static_cast<uint64_t>(std::abs(d)));

    std::shared_ptr<Instruction> x1 = std::make_shared<ASHR>(share(n), n.type()->constant(l - 1));
    std::shared_ptr<Instruction> x2 = std::make_shared<LSHR>(x1, n.type()->constant(N - l));
    std::shared_ptr<Instruction> x3 = std::make_shared<Add>(share(n), x2);
    std::shared_ptr<Instruction> x4 = std::make_shared<ASHR>(x3, n.type()->constant(l));

    replaced += x1, x2, x3, x4;

    if (d < 0) {
        std::shared_ptr<Instruction> x5 = std::make_shared<Sub>(n.type()->constant(0), x4);
        replaced += x5;
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSDivGeneral(Value &n, int64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced;

    int N = n.type()->bitSize();
    int l = std::max(N - std::countl_zero(static_cast<uint32_t>(std::abs(d) - 1)), 1);
    int32_t m = static_cast<int32_t>(1 + (UINT64_C(1) << (N + l - 1)) / std::abs(d) - (UINT64_C(1) << N));

    std::shared_ptr<Instruction> x1 = std::make_shared<SExt>(share(n), cast<IntegerType>(n.type()->promoted()));
    replaced += x1;

    std::shared_ptr<Instruction> x2;
    if (m == 1) {
        x2 = x1;
    } else {
        x2 = std::make_shared<Mul>(x1, n.type()->promoted()->constant(m));
        replaced += x2;
    }

    std::shared_ptr<Instruction> x3 = std::make_shared<LSHR>(x2, n.type()->promoted()->constant(N));
    std::shared_ptr<Instruction> x4 = std::make_shared<Trunc>(x3, cast<IntegerType>(n.type()));
    std::shared_ptr<Instruction> x5 = std::make_shared<Add>(share(n), x4);
    std::shared_ptr<Instruction> x6 = std::make_shared<ASHR>(x5, n.type()->constant(l - 1));
    std::shared_ptr<Instruction> x7 = std::make_shared<ASHR>(share(n), n.type()->constant(N - 1));
    std::shared_ptr<Instruction> x8 = std::make_shared<Sub>(x6, x7);

    replaced += x3, x4, x5, x6, x7, x8;

    if (d < 0) {
        std::shared_ptr<Instruction> x9 = std::make_shared<Sub>(n.type()->constant(0), x8);
        replaced += x9;
    }

    return replaced;
}

std::vector<std::shared_ptr<Instruction>> replaceSDiv(Value &n, int64_t d) {
    if (d == 0 || d == 1) {
        return {};
    }

    if (d == -1) {
        return replaceSDivNeg1(n);
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
    return std::vector<std::shared_ptr<Instruction>>() +
           std::make_shared<And>(share(n), n.type()->constant(d - 1));
}

std::vector<std::shared_ptr<Instruction>> replaceURemGeneral(Value &n, uint64_t d) {
    std::vector<std::shared_ptr<Instruction>> replaced = replaceUDiv(n, d);

    if (!replaced.empty()) {
        std::shared_ptr<Instruction> x = replaced.back();
        std::shared_ptr<Instruction> x1 = std::make_shared<Mul>(x, n.type()->constant(d));
        std::shared_ptr<Instruction> x2 = std::make_shared<Sub>(share(n), x1);

        replaced += x1, x2;
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
        std::shared_ptr<Instruction> x = replaced.back();
        std::shared_ptr<Instruction> x1 = std::make_shared<Mul>(x, n.type()->constant(d));
        std::shared_ptr<Instruction> x2 = std::make_shared<Sub>(share(n), x1);

        replaced += x1, x2;
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

void dfs(const DTNode *node, bool &changed, size_t mulThreshold, size_t divThreshold, size_t remThreshold) {
    for (auto i = node->block->begin(); i != node->block->end();) {
        const Instruction &I = *i++;

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
            replaceAllUsesWith(I, share(*result));
            removeFromParent(I);
            changed = true;
        }
    }

    for (const DTNode *child : node->children) {
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

    assert(F.isWellFormed());
    return changed;
}
