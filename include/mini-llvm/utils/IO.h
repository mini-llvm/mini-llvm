// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdio>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemError.h"

namespace mini_llvm {

MINI_LLVM_EXPORT Expected<std::string, SystemError> readAll(FILE *stream);
MINI_LLVM_EXPORT Expected<std::string, SystemError> readAll(const Path &path);
MINI_LLVM_EXPORT Expected<std::string, SystemError> readAll(const Path &path, FILE *stream);

MINI_LLVM_EXPORT Expected<void, SystemError> writeAll(FILE *stream, const char *data, size_t size);
MINI_LLVM_EXPORT Expected<void, SystemError> writeAll(const Path &path, const char *data, size_t size);
MINI_LLVM_EXPORT Expected<void, SystemError> writeAll(const Path &path, FILE *stream, const char *data, size_t size);

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
