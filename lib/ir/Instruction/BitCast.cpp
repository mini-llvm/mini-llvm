#include "mini-llvm/ir/Instruction/BitCast.h"

#include <cstdint>
#include <memory>
#include <optional>

#include "mini-llvm/common/ops/BitCast.h"
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
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/TypeVisitor.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

template <typename To, typename ToConst, typename ToTy>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    std::unique_ptr<Constant> takeResult() {
        return std::move(result_.value());
    }

    void visitI1Constant(const I1Constant &value) override {
        visit(value);
    }

    void visitI8Constant(const I8Constant &value) override {
        visit(value);
    }

    void visitI16Constant(const I16Constant &value) override {
        visit(value);
    }

    void visitI32Constant(const I32Constant &value) override {
        visit(value);
    }

    void visitI64Constant(const I64Constant &value) override {
        visit(value);
    }

    void visitFloatConstant(const FloatConstant &value) override {
        visit(value);
    }

    void visitDoubleConstant(const DoubleConstant &value) override {
        visit(value);
    }

    void visitNullPtrConstant(const NullPtrConstant &) override {
        result_.emplace(ToTy().zeroValue());
    }

private:
    std::optional<std::unique_ptr<Constant>> result_;

    template <typename FromConst>
    void visit(const FromConst &value) {
        result_.emplace(std::make_unique<ToConst>(ops::BitCast<To>()(value.value())));
    }
};

class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::unique_ptr<Constant> takeResult() {
        return std::move(result_.value());
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

    void visitFloat(const Float &) override {
        visit<float, FloatConstant, Float>();
    }

    void visitDouble(const Double &) override {
        visit<double, DoubleConstant, Double>();
    }

private:
    const Constant &value_;
    std::optional<std::unique_ptr<Constant>> result_;

    template <typename To, typename ToConst, typename ToTy>
    void visit() {
        ConstantVisitorImpl<To, ToConst, ToTy> visitor;
        value_.accept(visitor);
        result_.emplace(visitor.takeResult());
    }
};

std::unique_ptr<Constant> foldImpl(const BitCast &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_unique<PoisonValue>(I.type());
    }
    TypeVisitorImpl visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

} // namespace

std::unique_ptr<Constant> BitCast::fold() const {
    return foldImpl(*this);
}
