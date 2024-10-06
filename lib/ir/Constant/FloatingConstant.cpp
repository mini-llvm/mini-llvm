#include "mini-llvm/ir/Constant/FloatingConstant.h"

#include <memory>
#include <optional>
#include <utility>

#include "mini-llvm/common/ops/FPTrunc.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/TypeVisitor.h"

using namespace mini_llvm::ir;

namespace {

class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(double value) : value_(value) {}

    std::unique_ptr<FloatingConstant> takeResult() {
        return std::move(result_.value());
    }

    void visitFloat(const Float &) override {
        visit<FloatConstant, float>();
    }

    void visitDouble(const Double &) override {
        visit<DoubleConstant, double>();
    }

private:
    double value_;
    std::optional<std::unique_ptr<FloatingConstant>> result_;

    template <typename Const, typename To>
    void visit() {
        result_.emplace(std::make_unique<Const>(mini_llvm::ops::FPTrunc<To>()(value_)));
    }
};

} // namespace

std::unique_ptr<FloatingConstant> mini_llvm::ir::createFloatingConstant(std::unique_ptr<Type> type, double value) {
    TypeVisitorImpl visitor(value);
    type->accept(visitor);
    return visitor.takeResult();
}
