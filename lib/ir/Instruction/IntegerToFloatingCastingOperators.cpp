// SPDX-License-Identifier: MIT

#include <format>
#include <memory>
#include <optional>
#include <string>
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
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

template <typename Op, typename ResultConst>
class ConstantVisitorImpl : public ConstantVisitor {
public:
    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
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
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename Const>
    void visit(const Const &value) {
        result_.emplace(std::make_shared<ResultConst>(Op()(value.value())));
    }
};

template <template <typename To> typename Op>
class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitFloat(const Float &) override {
        visit<float, FloatConstant>();
    }

    void visitDouble(const Double &) override {
        visit<double, DoubleConstant>();
    }

private:
    const Constant &value_;
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename To, typename ResultConst>
    void visit() {
        ConstantVisitorImpl<Op<To>, ResultConst> visitor;
        value_.accept(visitor);
        result_.emplace(visitor.takeResult());
    }
};

template <template <typename To> typename Op>
std::shared_ptr<Constant> foldImpl(const IntegerToFloatingCastingOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_shared<PoisonValue>(I.type());
    }
    TypeVisitorImpl<Op> visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

std::string formatImpl(const IntegerToFloatingCastingOperator &I, const char *mnemonic) {
    return std::format("{:o} = {} {} {:o} to {}", I, mnemonic, *I.value()->type(), *I.value(), *I.type());
}

template <typename T>
std::unique_ptr<Value> cloneImpl(const IntegerToFloatingCastingOperator &I) {
    return std::make_unique<T>(share(*I.value()), cast<FloatingType>(I.type()));
}

} // namespace

std::shared_ptr<Constant> SIToFP::fold() const {
    return foldImpl<ops::SIToFP>(*this);
}

std::shared_ptr<Constant> UIToFP::fold() const {
    return foldImpl<ops::UIToFP>(*this);
}

std::string SIToFP::format() const {
    return formatImpl(*this, "sitofp");
}

std::string UIToFP::format() const {
    return formatImpl(*this, "uitofp");
}

std::unique_ptr<Value> SIToFP::clone() const {
    return cloneImpl<SIToFP>(*this);
}

std::unique_ptr<Value> UIToFP::clone() const {
    return cloneImpl<UIToFP>(*this);
}
