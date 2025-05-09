#include "mini-llvm/utils/FileSystem.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileHandle.h"

using namespace mini_llvm;

Expected<std::string, int> mini_llvm::readAll(const std::filesystem::path &path) {
    FileHandle handle(path.c_str(), "r");
    if (!handle) {
        return Unexpected(errno);
    }
    std::string content;
    std::string chunk(4096, '\0');
    for (;;) {
        size_t n = fread(chunk.data(), 1, chunk.size(), handle.get());
        content.append(chunk.data(), n);
        if (n < chunk.size()) {
            if (ferror(handle.get())) {
                return Unexpected(errno);
            }
            break;
        }
    }
    return content;
}

Expected<void, int> mini_llvm::writeAll(const std::filesystem::path &path, std::string_view content) {
    FileHandle handle(path.c_str(), "w");
    if (!handle) {
        return Unexpected(errno);
    }
    if (fwrite(content.data(), 1, content.size(), handle.get()) != content.size()) {
        return Unexpected(errno);
    }
    return {};
}
