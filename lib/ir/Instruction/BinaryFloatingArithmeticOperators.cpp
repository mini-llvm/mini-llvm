// SPDX-License-Identifier: MIT

#include <format>
#include <memory>
#include <optional>
#include <string>
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
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
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

std::string formatImpl(const BinaryFloatingArithmeticOperator &I, const char *mnemonic) {
    return std::format("{:o} = {} {} {:o}, {:o}", I, mnemonic, *I.lhs()->type(), *I.lhs(), *I.rhs());
}

template <typename T>
std::unique_ptr<Value> cloneImpl(const BinaryFloatingArithmeticOperator &I) {
    return std::make_unique<T>(share(*I.lhs()), share(*I.rhs()));
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

std::string FAdd::format() const {
    return formatImpl(*this, "fadd");
}

std::string FSub::format() const {
    return formatImpl(*this, "fsub");
}

std::string FMul::format() const {
    return formatImpl(*this, "fmul");
}

std::string FDiv::format() const {
    return formatImpl(*this, "fdiv");
}

std::string FRem::format() const {
    return formatImpl(*this, "frem");
}

std::unique_ptr<Value> FAdd::clone() const {
    return cloneImpl<FAdd>(*this);
}

std::unique_ptr<Value> FSub::clone() const {
    return cloneImpl<FSub>(*this);
}

std::unique_ptr<Value> FMul::clone() const {
    return cloneImpl<FMul>(*this);
}

std::unique_ptr<Value> FDiv::clone() const {
    return cloneImpl<FDiv>(*this);
}

std::unique_ptr<Value> FRem::clone() const {
    return cloneImpl<FRem>(*this);
}
