#pragma once

#include <cstdio>
#include <string>
#include <utility>

namespace mini_llvm {

class FileHandle {
public:
    FileHandle() = default;

    explicit FileHandle(FILE *handle) : handle_(handle) {}

    FileHandle(const char *path, const char *mode);

    FileHandle(const std::string &path, const char *mode)
        : FileHandle(path.c_str(), mode) {}

    FileHandle(const char *path, const std::string &mode)
        : FileHandle(path, mode.c_str()) {}

    FileHandle(const std::string &path, const std::string &mode)
        : FileHandle(path.c_str(), mode.c_str()) {}

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

    void open(const char *path, const char *mode);

    void open(const std::string &path, const char *mode) {
        open(path.c_str(), mode);
    }

    void open(const char *path, const std::string &mode) {
        open(path, mode.c_str());
    }

    void open(const std::string &path, const std::string &mode) {
        open(path.c_str(), mode.c_str());
    }

    void close();

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
