#include <memory>
#include <optional>
#include <utility>

#include "mini-llvm/common/ops/FAdd.h"
#include "mini-llvm/common/ops/FDiv.h"
#include "mini-llvm/common/ops/FMul.h"
#include "mini-llvm/common/ops/FRem.h"
#include "mini-llvm/common/ops/FSub.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/FAdd.h"
#include "mini-llvm/ir/Instruction/FDiv.h"
#include "mini-llvm/ir/Instruction/FMul.h"
#include "mini-llvm/ir/Instruction/FRem.h"
#include "mini-llvm/ir/Instruction/FSub.h"

using namespace mini_llvm::ir;

namespace {

template <typename Op>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    explicit ConstantVisitorImpl(const Constant &lhs) : lhs_(lhs) {}

    std::shared_ptr<Constant> takeResult() {
        return std::move(*result_);
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
        result_.emplace(std::make_shared<Const>(Op()(static_cast<const Const &>(lhs_).value(), rhs.value())));
    }
};

template <typename Op>
std::shared_ptr<Constant> foldImpl(const BinaryFloatingArithmeticOperator &I) {
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

std::shared_ptr<Constant> FAdd::fold() const {
    return foldImpl<ops::FAdd>(*this);
}

std::shared_ptr<Constant> FSub::fold() const {
    return foldImpl<ops::FSub>(*this);
}

std::shared_ptr<Constant> FMul::fold() const {
    return foldImpl<ops::FMul>(*this);
}

std::shared_ptr<Constant> FDiv::fold() const {
    return foldImpl<ops::FDiv>(*this);
}

std::shared_ptr<Constant> FRem::fold() const {
    return foldImpl<ops::FRem>(*this);
}
