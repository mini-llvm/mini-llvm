#pragma once

#include <cassert>
#include <cstdlib>

namespace mini_llvm {

[[noreturn]] inline void panic(int = (assert(false && "panic()"), 0)) {
    abort();
}

} // namespace mini_llvm
