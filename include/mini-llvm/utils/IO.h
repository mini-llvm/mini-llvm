#pragma once

#include <cstddef>
#include <cstdio>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemError.h"

namespace mini_llvm {

Expected<std::string, SystemError> readAll(FILE *stream);
Expected<std::string, SystemError> readAll(const Path &path);
Expected<std::string, SystemError> readAll(const Path &path, FILE *stream);

Expected<void, SystemError> writeAll(FILE *stream, const char *data, size_t size);
Expected<void, SystemError> writeAll(const Path &path, const char *data, size_t size);
Expected<void, SystemError> writeAll(const Path &path, FILE *stream, const char *data, size_t size);

inline Expected<void, SystemError> writeAll(FILE *stream, std::string_view buffer) {
    return writeAll(stream, buffer.data(), buffer.size());
}

inline Expected<void, SystemError> writeAll(const Path &path, std::string_view buffer) {
    return writeAll(path, buffer.data(), buffer.size());
}

inline Expected<void, SystemError> writeAll(const Path &path, FILE *stream, std::string_view buffer) {
    return writeAll(path, stream, buffer.data(), buffer.size());
}

} // namespace mini_llvm
