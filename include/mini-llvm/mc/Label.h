#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Line.h"

namespace mini_llvm::mc {

class Label final : public Line {
public:
    explicit Label(std::string labelName) : labelName_(std::move(labelName)) {}

    const std::string &labelName() const {
        return labelName_;
    }

    void setLabelName(std::string name) {
        labelName_ = std::move(name);
    }

    std::string format() const override {
        return labelName() + ":";
    }

private:
    std::string labelName_;
};

} // namespace mini_llvm::mc
