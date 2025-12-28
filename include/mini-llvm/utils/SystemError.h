// SPDX-License-Identifier: MIT

#pragma once

#include <optional>

#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/ErrorCode.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT SystemError {
public:
#ifdef _WIN32
    using native_type = unsigned long;
#else
    using native_type = int;
#endif

    explicit SystemError(ErrorCode code)
        : code_(code) {}

    SystemError(ErrorCode code, native_type native)
        : code_(code), native_(native) {}

    ErrorCode code() const {
        return code_;
    }

    std::optional<native_type> native() const {
        return native_;
    }

    static SystemError fromNative(native_type native);

private:
    ErrorCode code_;
    std::optional<native_type> native_;
};

} // namespace mini_llvm
