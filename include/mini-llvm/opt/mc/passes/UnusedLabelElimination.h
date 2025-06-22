#pragma once

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/opt/mc/GlobalValueTransform.h"

namespace mini_llvm::mc {

class UnusedLabelElimination final : public GlobalValueTransform {
public:
    bool runOnGlobalValue(GlobalValue &G) override;

    const char *name() const override {
        return "UnusedLabelElimination";
    }
};

} // namespace mini_llvm::mc
