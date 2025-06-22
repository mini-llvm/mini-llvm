#pragma once

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/opt/mc/FragmentTransform.h"

namespace mini_llvm::mc {

class UnusedLabelElimination final : public FragmentTransform {
public:
    bool runOnFragment(Fragment &fragment) override;

    const char *name() const override {
        return "UnusedLabelElimination";
    }
};

} // namespace mini_llvm::mc
