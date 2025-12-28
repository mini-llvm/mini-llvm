// SPDX-License-Identifier: MIT

#pragma once

#include <cstdio>
#include <utility>

#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemString.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT FileHandle {
public:
    FileHandle() = default;

    explicit FileHandle(FILE *handle) : handle_(handle) {}

    FileHandle(const Path &path, const SystemString &mode);

    ~FileHandle();

    FileHandle(const FileHandle &) = delete;

    FileHandle(FileHandle &&other) noexcept {
        swap(other);
    }

    FileHandle &operator=(FileHandle other) noexcept {
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

    void swap(FileHandle &other) noexcept {
        std::swap(handle_, other.handle_);
    }

private:
    FILE *handle_{};
};

inline void swap(FileHandle &lhs, FileHandle &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mini_llvm
