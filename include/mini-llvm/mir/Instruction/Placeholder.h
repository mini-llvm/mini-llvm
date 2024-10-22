#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/RegisterOperand.h"

namespace mini_llvm::mir {

class Placeholder : public Instruction {
public:
    explicit Placeholder(int id) : id_(id) {}

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

    bool hasSideEffects() const override {
        return false;
    }

    std::string format() const override {
        return std::format("PLACEHOLDER {}", id());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Placeholder>(id());
    }

    void accept(InstructionVisitor &) override {}

    void accept(InstructionVisitor &) const override {}

private:
    int id_;
};

} // namespace mini_llvm::mir
