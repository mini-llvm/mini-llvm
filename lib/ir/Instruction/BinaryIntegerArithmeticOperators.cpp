#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

#include "mini-llvm/common/ops/Add.h"
#include "mini-llvm/common/ops/And.h"
#include "mini-llvm/common/ops/ASHR.h"
#include "mini-llvm/common/ops/LSHR.h"
#include "mini-llvm/common/ops/Mul.h"
#include "mini-llvm/common/ops/Or.h"
#include "mini-llvm/common/ops/SDiv.h"
#include "mini-llvm/common/ops/SHL.h"
#include "mini-llvm/common/ops/SRem.h"
#include "mini-llvm/common/ops/Sub.h"
#include "mini-llvm/common/ops/UDiv.h"
#include "mini-llvm/common/ops/URem.h"
#include "mini-llvm/common/ops/Xor.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/And.h"
#include "mini-llvm/ir/Instruction/ASHR.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/Or.h"
#include "mini-llvm/ir/Instruction/SDiv.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Instruction/SRem.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Instruction/UDiv.h"
#include "mini-llvm/ir/Instruction/URem.h"
#include "mini-llvm/ir/Instruction/Xor.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template <typename Op>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    explicit ConstantVisitorImpl(const Constant &lhs) : lhs_(lhs) {}

    std::shared_ptr<Constant> takeResult() {
        return std::move(*result_);
    }

    void visitI1Constant(const I1Constant &rhs) override {
        visit<I1Constant, I1>(rhs);
    }

    void visitI8Constant(const I8Constant &rhs) override {
        visit<I8Constant, I8>(rhs);
    }

    void visitI16Constant(const I16Constant &rhs) override {
        visit<I16Constant, I16>(rhs);
    }

    void visitI32Constant(const I32Constant &rhs) override {
        visit<I32Constant, I32>(rhs);
    }

    void visitI64Constant(const I64Constant &rhs) override {
        visit<I64Constant, I64>(rhs);
    }

private:
    const Constant &lhs_;
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename Const, typename Ty>
    void visit(const Const &rhs) {
        auto opResult = Op()(static_cast<const Const &>(lhs_).value(), rhs.value());
        if constexpr (is_optional_v<decltype(opResult)>) {
            if (opResult) {
                result_.emplace(std::make_shared<Const>(*opResult));
            } else {
                result_.emplace(std::make_shared<PoisonValue>(std::make_unique<Ty>()));
            }
        } else {
            result_.emplace(std::make_shared<Const>(opResult));
        }
    }
};

template <typename Op>
std::shared_ptr<Constant> foldImpl(const BinaryIntegerArithmeticOperator &I) {
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

std::shared_ptr<Constant> Add::fold() const {
    return foldImpl<ops::Add>(*this);
}

std::shared_ptr<Constant> Sub::fold() const {
    return foldImpl<ops::Sub>(*this);
}

std::shared_ptr<Constant> Mul::fold() const {
    return foldImpl<ops::Mul>(*this);
}

std::shared_ptr<Constant> SDiv::fold() const {
    return foldImpl<ops::SDiv>(*this);
}

std::shared_ptr<Constant> UDiv::fold() const {
    return foldImpl<ops::UDiv>(*this);
}

std::shared_ptr<Constant> SRem::fold() const {
    return foldImpl<ops::SRem>(*this);
}

std::shared_ptr<Constant> URem::fold() const {
    return foldImpl<ops::URem>(*this);
}

std::shared_ptr<Constant> And::fold() const {
    return foldImpl<ops::And>(*this);
}

std::shared_ptr<Constant> Or::fold() const {
    return foldImpl<ops::Or>(*this);
}

std::shared_ptr<Constant> Xor::fold() const {
    return foldImpl<ops::Xor>(*this);
}

std::shared_ptr<Constant> SHL::fold() const {
    return foldImpl<ops::SHL>(*this);
}

std::shared_ptr<Constant> LSHR::fold() const {
    return foldImpl<ops::LSHR>(*this);
}

std::shared_ptr<Constant> ASHR::fold() const {
    return foldImpl<ops::ASHR>(*this);
}
