#pragma once

#include <cstddef>

namespace mini_llvm {

void computeLineColumn(const char *start, const char *location, size_t &line, size_t &column);

} // namespace mini_llvm
