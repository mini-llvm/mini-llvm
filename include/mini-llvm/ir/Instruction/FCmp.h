// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingRelationalOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FCmp final : public BinaryFloatingRelationalOperator {
public:
    enum class Condition {
        kOEQ,
        kONE,
        kOLT,
        kOGT,
        kOLE,
        kOGE,
    };

    FCmp(Condition cond, std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryFloatingRelationalOperator(std::move(lhs), std::move(rhs)), cond_(cond) {}

    Condition cond() const {
        return cond_;
    }

    void setCond(Condition cond) {
        cond_ = cond;
    }

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFCmp(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFCmp(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Condition cond_;
};

inline constexpr const char *specifier(FCmp::Condition cond) {
    using enum FCmp::Condition;
    switch (cond) {
        case kOEQ: return "oeq";
        case kONE: return "one";
        case kOLT: return "olt";
        case kOGT: return "ogt";
        case kOLE: return "ole";
        case kOGE: return "oge";
        default: abort();
    }
}

} // namespace mini_llvm::ir
