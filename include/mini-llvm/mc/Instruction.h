// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/mc/Statement.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Instruction : public Statement {
public:
    using operand_iterator = IndirectIterator<std::vector<std::unique_ptr<Operand>>::iterator, Operand>;
    using const_operand_iterator = IndirectIterator<std::vector<std::unique_ptr<Operand>>::const_iterator, const Operand>;

    int opcode() const {
        return opcode_;
    }

    void setOpcode(int opcode) {
        opcode_ = opcode;
    }

    operand_iterator operand_begin() {
        return operand_iterator(operands_.begin());
    }

    operand_iterator operand_end() {
        return operand_iterator(operands_.end());
    }

    const_operand_iterator operand_begin() const {
        return const_operand_iterator(operands_.begin());
    }

    const_operand_iterator operand_end() const {
        return const_operand_iterator(operands_.end());
    }

    bool operand_empty() const {
        return operands_.empty();
    }

    size_t operand_size() const {
        return operands_.size();
    }

    Operand &operand(size_t i) {
        return *operands_[i];
    }

    const Operand &operand(size_t i) const {
        return *operands_[i];
    }

    void setOperands(std::vector<std::unique_ptr<Operand>> operands) {
        operands_ = std::move(operands);
    }

protected:
    explicit Instruction(int opcode, std::vector<std::unique_ptr<Operand>> operands = {})
        : opcode_(opcode), operands_(std::move(operands)) {}

private:
    int opcode_;
    std::vector<std::unique_ptr<Operand>> operands_;
};

inline auto operands(Instruction &I) {
    return std::ranges::subrange(I.operand_begin(), I.operand_end());
}

inline auto operands(const Instruction &I) {
    return std::ranges::subrange(I.operand_begin(), I.operand_end());
}

} // namespace mini_llvm::mc
