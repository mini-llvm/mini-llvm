#pragma once

#include <cstdio>
#include <cstdlib>

namespace mini_llvm {

[[noreturn]] inline void panic() {
    fprintf(stderr, "mini_llvm::panic()\n");
    abort();
}

} // namespace mini_llvm
