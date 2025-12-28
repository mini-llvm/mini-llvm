// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FPExt final : public FloatingCastingOperator {
public:
    FPExt(std::shared_ptr<Value> value, std::unique_ptr<FloatingType> type)
        : FloatingCastingOperator(std::move(value), std::move(type)) {}

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFPExt(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFPExt(*this);
    }

    bool isWellFormed() const override;
    std::string format() const override;
    std::unique_ptr<Value> clone() const override;
};

} // namespace mini_llvm::ir
