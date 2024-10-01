#pragma once

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/opt/mc/FragmentTransform.h"

namespace mini_llvm::mc {

class RedundantLabelElimination final : public FragmentTransform {
public:
    bool runOnFragment(Fragment &fragment) override;
};

} // namespace mini_llvm::mc
