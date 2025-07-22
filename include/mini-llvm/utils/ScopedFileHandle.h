#pragma once

#include <cstdio>
#include <utility>

#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemString.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT ScopedFileHandle {
public:
    ScopedFileHandle() = default;

    explicit ScopedFileHandle(FILE *handle) : handle_(handle) {}

    ScopedFileHandle(const Path &path, const SystemString &mode);

    ~ScopedFileHandle();

    ScopedFileHandle(const ScopedFileHandle &) = delete;

    ScopedFileHandle(ScopedFileHandle &&other) noexcept {
        swap(other);
    }

    ScopedFileHandle &operator=(ScopedFileHandle other) noexcept {
        swap(other);
        return *this;
    }

    explicit operator bool() const {
        return handle_;
    }

    FILE *get() {
        return handle_;
    }

    const FILE *get() const {
        return handle_;
    }

    void open(const Path &path, const SystemString &mode);

    void close();

    FILE *release();

    void swap(ScopedFileHandle &other) noexcept {
        std::swap(handle_, other.handle_);
    }

private:
    FILE *handle_{};
};

inline void swap(ScopedFileHandle &lhs, ScopedFileHandle &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mini_llvm
