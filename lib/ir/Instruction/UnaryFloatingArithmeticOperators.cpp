#include <memory>
#include <optional>
#include <utility>

#include "mini-llvm/common/ops/FNeg.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/FNeg.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingArithmeticOperator.h"

using namespace mini_llvm::ir;

namespace {

template <typename Op>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    std::unique_ptr<Constant> takeResult() {
        return std::move(result_.value());
    }

    void visitFloatConstant(const FloatConstant &value) override {
        visit<FloatConstant>(value);
    }

    void visitDoubleConstant(const DoubleConstant &value) override {
        visit<DoubleConstant>(value);
    }

private:
    std::optional<std::unique_ptr<Constant>> result_;

    template <typename Const>
    void visit(const Const &value) {
        result_.emplace(std::make_unique<Const>(Op()(value.value())));
    }
};

template <typename Op>
std::unique_ptr<Constant> foldImpl(const UnaryFloatingArithmeticOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_unique<PoisonValue>(I.type());
    }
    ConstantVisitorImpl<Op> visitor;
    value.accept(visitor);
    return visitor.takeResult();
}

} // namespace

std::unique_ptr<Constant> FNeg::fold() const {
    return foldImpl<ops::FNeg>(*this);
}
