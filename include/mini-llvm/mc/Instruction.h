#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mc/Line.h"
#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::mc {

class Instruction : public Line {
public:
    using operand_iterator = IndirectIterator<std::vector<std::unique_ptr<Operand>>::iterator, Operand>;
    using const_operand_iterator = IndirectIterator<std::vector<std::unique_ptr<Operand>>::const_iterator, const Operand>;

    explicit Instruction(int opcode, std::vector<std::unique_ptr<Operand>> operands = {})
        : opcode_(opcode), operands_(std::move(operands)) {}

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

    void setOperands(std::vector<std::unique_ptr<Operand>> operands) {
        operands_ = std::move(operands);
    }

    std::string format() const override;

protected:
    virtual std::string mnemonic(int opcode) const = 0;

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
