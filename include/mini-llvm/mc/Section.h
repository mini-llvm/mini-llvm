#pragma once

namespace mini_llvm::mc {

enum class Section {
    kData,
    kROData,
    kBSS,
    kText,
};

} // namespace mini_llvm::mc
