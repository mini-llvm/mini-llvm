#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/GlobalVarOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class LA : public Instruction {
public:
    LA(int width, std::shared_ptr<Register> dst, GlobalVar *src)
        : width_(width), dst_(RegisterKind::kInteger, std::move(dst)), src_(src) {}

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

    std::unordered_set<const mir::RegisterOperand *> regOps() const override {
        return {&dst()};
    }

    std::unordered_set<const mir::RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const mir::RegisterOperand *> srcs() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return false;
    }

    std::string format() const override {
        return std::format("LA<{}> {}, {:o}", width(), *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<LA>(width(), share(*dst()), &*src());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLA(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLA(*this);
    }

private:
    int width_;
    RegisterOperand dst_;
    GlobalVarOperand src_;
};

} // namespace mini_llvm::mir
