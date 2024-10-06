#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Operand.h"

namespace mini_llvm::mc {

class LabelOperand final : public Operand {
public:
    explicit LabelOperand(std::string labelName) : labelName_(std::move(labelName)) {}

    const std::string &labelName() const {
        return labelName_;
    }

    void setLabelName(std::string name) {
        labelName_ = std::move(name);
    }

    std::string format() const override {
        return labelName();
    }

private:
    std::string labelName_;
};

} // namespace mini_llvm::mc
