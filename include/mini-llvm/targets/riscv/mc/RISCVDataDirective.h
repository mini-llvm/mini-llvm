#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "mini-llvm/mc/DataDirective.h"

namespace mini_llvm::mc {

class RISCVDataDirective final : public DataDirective {
public:
    RISCVDataDirective(int width, int64_t value)
        : DataDirective(width, value) {}

protected:
    std::string directiveName(int width) const override {
        switch (width) {
            case 1: return "byte";
            case 2: return "half";
            case 4: return "word";
            case 8: return "dword";
            default: std::unreachable();
        }
    }
};

} // namespace mini_llvm::mc
