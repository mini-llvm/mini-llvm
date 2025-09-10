// SPDX-License-Identifier: MIT

#pragma once

#include <cerrno>

namespace mini_llvm {

enum class ErrorCode {
#define X(name, value, message) k##name = value,
#include "mini-llvm/utils/ErrorCode.def"
#undef X
};

inline const char *message(ErrorCode code) {
    using enum ErrorCode;

#define X(name, value, message) if (code == k##name) return message;
#include "mini-llvm/utils/ErrorCode.def"
#undef X

    return "Unknown error";
}

} // namespace mini_llvm
