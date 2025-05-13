#pragma once

#include <string>

#include "mini-llvm/utils/Expected.h"

namespace mini_llvm {

Expected<std::string, int> readAll(const char *path);

inline Expected<std::string, int> readAll(const std::string &path) {
    return readAll(path.c_str());
}

Expected<void, int> writeAll(const char *path, const char *data, size_t size);

inline Expected<void, int> writeAll(const std::string &path, const char *data, size_t size) {
    return writeAll(path.c_str(), data, size);
}

} // namespace mini_llvm
