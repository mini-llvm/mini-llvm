// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Marker : public Instruction {
public:
    explicit Marker(int id) : id_(id) {}

    int id() const {
        return id_;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("MARKER {}", id());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Marker>(id());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitMarker(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitMarker(*this);
    }

private:
    int id_;
};

} // namespace mini_llvm::mir
