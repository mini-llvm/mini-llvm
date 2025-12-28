// SPDX-License-Identifier: MIT

#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/ops/FPToSI.h"
#include "mini-llvm/common/ops/FPToUI.h"
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
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/FPToSI.h"
#include "mini-llvm/ir/Instruction/FPToUI.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

template <typename Op, typename ResultConst, typename ResultTy>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitFloatConstant(const FloatConstant &value) override {
        visit<FloatConstant>(value);
    }

    void visitDoubleConstant(const DoubleConstant &value) override {
        visit<DoubleConstant>(value);
    }

private:
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename Const>
    void visit(const Const &value) {
        auto opResult = Op()(value.value());
        if (opResult) {
            result_.emplace(std::make_shared<ResultConst>(*opResult));
        } else {
            result_.emplace(std::make_shared<PoisonValue>(std::make_unique<ResultTy>()));
        }
    }
};

template <template <typename To> typename Op>
class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitI1(const I1 &) override {
        visit<bool, I1Constant, I1>();
    }

    void visitI8(const I8 &) override {
        visit<int8_t, I8Constant, I8>();
    }

    void visitI16(const I16 &) override {
        visit<int16_t, I16Constant, I16>();
    }

    void visitI32(const I32 &) override {
        visit<int32_t, I32Constant, I32>();
    }

    void visitI64(const I64 &) override {
        visit<int64_t, I64Constant, I64>();
    }

private:
    const Constant &value_;
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename To, typename ResultConst, typename ResultTy>
    void visit() {
        ConstantVisitorImpl<Op<To>, ResultConst, ResultTy> visitor;
        value_.accept(visitor);
        result_ = visitor.takeResult();
    }
};

template <template <typename To> typename Op>
std::shared_ptr<Constant> foldImpl(const FloatingToIntegerCastingOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_shared<PoisonValue>(I.type());
    }
    TypeVisitorImpl<Op> visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

std::string formatImpl(const FloatingToIntegerCastingOperator &I, const char *mnemonic) {
    return std::format("{:o} = {} {} {:o} to {}", I, mnemonic, *I.value()->type(), *I.value(), *I.type());
}

template <typename T>
std::unique_ptr<Value> cloneImpl(const FloatingToIntegerCastingOperator &I) {
    return std::make_unique<T>(share(*I.value()), cast<IntegerType>(I.type()));
}

} // namespace

std::shared_ptr<Constant> FPToSI::fold() const {
    return foldImpl<ops::FPToSI>(*this);
}

std::shared_ptr<Constant> FPToUI::fold() const {
    return foldImpl<ops::FPToUI>(*this);
}

std::string FPToSI::format() const {
    return formatImpl(*this, "fptosi");
}

std::string FPToUI::format() const {
    return formatImpl(*this, "fptoui");
}

std::unique_ptr<Value> FPToSI::clone() const {
    return cloneImpl<FPToSI>(*this);
}

std::unique_ptr<Value> FPToUI::clone() const {
    return cloneImpl<FPToUI>(*this);
}
