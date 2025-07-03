#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/Path.h"

namespace mini_llvm {

Expected<std::string, int> readAll(FILE *stream);
Expected<std::string, int> readAll(const Path &path);

Expected<void, int> writeAll(FILE *stream, const char *data, size_t size);
Expected<void, int> writeAll(const Path &path, const char *data, size_t size);

} // namespace mini_llvm
