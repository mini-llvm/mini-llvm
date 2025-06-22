#pragma once

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/opt/mc/GlobalValueTransform.h"

namespace mini_llvm::mc {

class RISCVFallthrough final : public GlobalValueTransform {
public:
    bool runOnGlobalValue(GlobalValue &G) override;

    const char *name() const override {
        return "RISCVFallthrough";
    }
};

} // namespace mini_llvm::mc
