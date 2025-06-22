#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Statement.h"

namespace mini_llvm::mc {

class Label final : public Statement {
public:
    explicit Label(std::string labelName) : labelName_(std::move(labelName)) {}

    const std::string &labelName() const {
        return labelName_;
    }

    void setLabelName(std::string labelName) {
        labelName_ = std::move(labelName);
    }

    std::string format() const override {
        return labelName() + ":";
    }

private:
    std::string labelName_;
};

} // namespace mini_llvm::mc
