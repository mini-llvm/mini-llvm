// SPDX-License-Identifier: MIT

#include <cassert>
#include <cstdlib>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/ops/EQ.h"
#include "mini-llvm/common/ops/NE.h"
#include "mini-llvm/common/ops/SGE.h"
#include "mini-llvm/common/ops/SGT.h"
#include "mini-llvm/common/ops/SLE.h"
#include "mini-llvm/common/ops/SLT.h"
#include "mini-llvm/common/ops/UGE.h"
#include "mini-llvm/common/ops/UGT.h"
#include "mini-llvm/common/ops/ULE.h"
#include "mini-llvm/common/ops/ULT.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerRelationalOperator.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

namespace {

template <typename Op>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    explicit ConstantVisitorImpl(const Constant &lhs) : lhs_(lhs) {}

    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitI1Constant(const I1Constant &rhs) override {
        visit<I1Constant>(rhs);
    }

    void visitI8Constant(const I8Constant &rhs) override {
        visit<I8Constant>(rhs);
    }

    void visitI16Constant(const I16Constant &rhs) override {
        visit<I16Constant>(rhs);
    }

    void visitI32Constant(const I32Constant &rhs) override {
        visit<I32Constant>(rhs);
    }

    void visitI64Constant(const I64Constant &rhs) override {
        visit<I64Constant>(rhs);
    }

    void visitNullPtrConstant(const NullPtrConstant &) override {
        assert(dynamic_cast<const NullPtrConstant *>(&lhs_));
        result_.emplace(std::make_shared<I1Constant>(Op()(0, 0)));
    }

private:
    const Constant &lhs_;
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename Const>
    void visit(const Const &rhs) {
        result_.emplace(std::make_shared<I1Constant>(Op()(static_cast<const Const &>(lhs_).value(), rhs.value())));
    }
};

template <typename Op>
std::shared_ptr<Constant> foldImpl(const BinaryIntegerRelationalOperator &I) {
    const Constant &lhs = static_cast<const Constant &>(*I.lhs()),
                   &rhs = static_cast<const Constant &>(*I.rhs());
    if (dynamic_cast<const PoisonValue *>(&lhs) || dynamic_cast<const PoisonValue *>(&rhs)) {
        return std::make_shared<PoisonValue>(I.type());
    }
    ConstantVisitorImpl<Op> visitor(lhs);
    rhs.accept(visitor);
    return visitor.takeResult();
}

} // namespace

std::shared_ptr<Constant> ICmp::fold() const {
    using enum Condition;
    switch (cond()) {
        case kEQ: return foldImpl<ops::EQ>(*this);
        case kNE: return foldImpl<ops::NE>(*this);
        case kSLT: return foldImpl<ops::SLT>(*this);
        case kSGT: return foldImpl<ops::SGT>(*this);
        case kSLE: return foldImpl<ops::SLE>(*this);
        case kSGE: return foldImpl<ops::SGE>(*this);
        case kULT: return foldImpl<ops::ULT>(*this);
        case kUGT: return foldImpl<ops::UGT>(*this);
        case kULE: return foldImpl<ops::ULE>(*this);
        case kUGE: return foldImpl<ops::UGE>(*this);
        default: abort();
    }
}

std::string ICmp::format() const {
    return std::format("{:o} = icmp {} {} {:o}, {:o}", *this, specifier(cond()), *lhs()->type(), *lhs(), *rhs());
}

std::unique_ptr<Value> ICmp::clone() const {
    return std::make_unique<ICmp>(cond(), share(*lhs()), share(*rhs()));
}
