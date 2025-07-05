#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemError.h"

namespace mini_llvm {

Expected<std::string, SystemError> readAll(FILE *stream);
Expected<std::string, SystemError> readAll(const Path &path);

Expected<void, SystemError> writeAll(FILE *stream, const char *data, size_t size);
Expected<void, SystemError> writeAll(const Path &path, const char *data, size_t size);

} // namespace mini_llvm
