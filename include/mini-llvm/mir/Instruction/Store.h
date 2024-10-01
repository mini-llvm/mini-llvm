#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class Store : public Instruction {
public:
    Store(int width, MemoryOperand dst, std::shared_ptr<Register> src)
        : width_(width), dst_(std::move(dst)), src_(RegisterKind::kInteger, std::move(src)) {}

    int width() const {
        return width_;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {&dst().baseReg(), &src()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&dst().baseReg(), &src()};
    }

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("STORE<{}> {}, {}", width(), dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Store>(width(), dst().clone(), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitStore(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitStore(*this);
    }

private:
    int width_;
    MemoryOperand dst_;
    RegisterOperand src_;
};

} // namespace mini_llvm::mir
