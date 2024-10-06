#include <memory>
#include <optional>
#include <utility>

#include "mini-llvm/common/ops/SIToFP.h"
#include "mini-llvm/common/ops/UIToFP.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/SIToFP.h"
#include "mini-llvm/ir/Instruction/UIToFP.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/TypeVisitor.h"

using namespace mini_llvm::ir;

namespace mini_llvm::ir {

template <typename Op, typename ResultConst>
class ConstantVisitorImpl : public ConstantVisitor {
public:
    std::unique_ptr<Constant> takeResult() {
        return std::move(result_.value());
    }

    void visitI1Constant(const I1Constant &value) override {
        visit<I1Constant>(value);
    }

    void visitI8Constant(const I8Constant &value) override {
        visit<I8Constant>(value);
    }

    void visitI16Constant(const I16Constant &value) override {
        visit<I16Constant>(value);
    }

    void visitI32Constant(const I32Constant &value) override {
        visit<I32Constant>(value);
    }

    void visitI64Constant(const I64Constant &value) override {
        visit<I64Constant>(value);
    }

private:
    std::optional<std::unique_ptr<Constant>> result_;

    template <typename Const>
    void visit(const Const &value) {
        result_.emplace(std::make_unique<ResultConst>(Op()(value.value())));
    }
};

template <template <typename To> typename Op>
class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::unique_ptr<Constant> takeResult() {
        return std::move(result_.value());
    }

    void visitFloat(const Float &) override {
        visit<float, FloatConstant>();
    }

    void visitDouble(const Double &) override {
        visit<double, DoubleConstant>();
    }

private:
    const Constant &value_;
    std::optional<std::unique_ptr<Constant>> result_;

    template <typename To, typename ResultConst>
    void visit() {
        ConstantVisitorImpl<Op<To>, ResultConst> visitor;
        value_.accept(visitor);
        result_.emplace(visitor.takeResult());
    }
};

template <template <typename To> typename Op>
std::unique_ptr<Constant> foldImpl(const IntegerToFloatingCastingOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_unique<PoisonValue>(I.type());
    }
    TypeVisitorImpl<Op> visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

} // namespace mini_llvm::ir

std::unique_ptr<Constant> SIToFP::fold() const {
    return foldImpl<ops::SIToFP>(*this);
}

std::unique_ptr<Constant> UIToFP::fold() const {
    return foldImpl<ops::UIToFP>(*this);
}
