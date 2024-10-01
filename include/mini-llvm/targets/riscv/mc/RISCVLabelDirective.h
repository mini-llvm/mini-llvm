#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/LabelDirective.h"

namespace mini_llvm::mc {

class RISCVLabelDirective final : public LabelDirective {
public:
    explicit RISCVLabelDirective(std::string labelName)
        : LabelDirective(std::move(labelName)) {}

protected:
    std::string directiveName() const override {
        return "dword";
    }
};

} // namespace mini_llvm::mc
