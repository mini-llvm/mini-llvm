#pragma once

#include <optional>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace mini_llvm {

class SystemError {
public:
#ifdef _WIN32
    using native_type = DWORD;
#else
    using native_type = int;
#endif

    explicit SystemError(int code)
        : code_(code) {}

    SystemError(int code, native_type native)
        : code_(code), native_(native) {}

    int code() const {
        return code_;
    }

    std::optional<native_type> native() const {
        return native_;
    }

    static SystemError fromNative(native_type native);

private:
    int code_;
    std::optional<native_type> native_;
};

} // namespace mini_llvm
