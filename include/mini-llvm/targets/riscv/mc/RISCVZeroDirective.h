#pragma once

#include <string>

#include "mini-llvm/mc/ZeroDirective.h"

namespace mini_llvm::mc {

class RISCVZeroDirective final : public ZeroDirective {
public:
    explicit RISCVZeroDirective(int size) : ZeroDirective(size) {}

protected:
    std::string directiveName() const override {
        return "zero";
    }
};

} // namespace mini_llvm::mc
