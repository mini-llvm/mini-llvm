// SPDX-License-Identifier: MIT

#include <cstdlib>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/ops/OEQ.h"
#include "mini-llvm/common/ops/OGE.h"
#include "mini-llvm/common/ops/OGT.h"
#include "mini-llvm/common/ops/OLE.h"
#include "mini-llvm/common/ops/OLT.h"
#include "mini-llvm/common/ops/ONE.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingRelationalOperator.h"
#include "mini-llvm/ir/Instruction/FCmp.h"
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

    void visitFloatConstant(const FloatConstant &rhs) override {
        visit<FloatConstant>(rhs);
    }

    void visitDoubleConstant(const DoubleConstant &rhs) override {
        visit<DoubleConstant>(rhs);
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
std::shared_ptr<Constant> foldImpl(const BinaryFloatingRelationalOperator &I) {
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

std::shared_ptr<Constant> FCmp::fold() const {
    using enum Condition;
    switch (cond()) {
        case kOEQ: return foldImpl<ops::OEQ>(*this);
        case kONE: return foldImpl<ops::ONE>(*this);
        case kOLT: return foldImpl<ops::OLT>(*this);
        case kOGT: return foldImpl<ops::OGT>(*this);
        case kOLE: return foldImpl<ops::OLE>(*this);
        case kOGE: return foldImpl<ops::OGE>(*this);
        default: abort();
    }
}

std::string FCmp::format() const {
    return std::format("{:o} = fcmp {} {} {:o}, {:o}", *this, specifier(cond()), *lhs()->type(), *lhs(), *rhs());
}

std::unique_ptr<Value> FCmp::clone() const {
    return std::make_unique<FCmp>(cond(), share(*lhs()), share(*rhs()));
}
