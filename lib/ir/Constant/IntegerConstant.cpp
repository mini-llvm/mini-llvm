#include "mini-llvm/ir/Constant/IntegerConstant.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <utility>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/TypeVisitor.h"

using namespace mini_llvm::ir;

namespace {

class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(int64_t value) : value_(value) {}

    std::unique_ptr<IntegerConstant> takeResult() {
        return std::move(result_.value());
    }

    void visitI1(const I1 &) override {
        visit<I1Constant, bool>();
    }

    void visitI8(const I8 &) override {
        visit<I8Constant, int8_t>();
    }

    void visitI16(const I16 &) override {
        visit<I16Constant, int16_t>();
    }

    void visitI32(const I32 &) override {
        visit<I32Constant, int32_t>();
    }

    void visitI64(const I64 &) override {
        visit<I64Constant, int64_t>();
    }

private:
    int64_t value_;
    std::optional<std::unique_ptr<IntegerConstant>> result_;

    template <typename Const, typename To>
    void visit() {
        result_.emplace(std::make_unique<Const>(mini_llvm::ops::Trunc<To>()(value_)));
    }
};

} // namespace

std::unique_ptr<IntegerConstant> mini_llvm::ir::createIntegerConstant(std::unique_ptr<Type> type, int64_t value) {
    TypeVisitorImpl visitor(value);
    type->accept(visitor);
    return visitor.takeResult();
}
