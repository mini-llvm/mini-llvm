#include "mini-llvm/utils/FileSystem.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileHandle.h"

using namespace mini_llvm;

Expected<std::string, int> mini_llvm::readAll(const char *path) {
    FileHandle handle(path, "r");
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

Expected<void, int> mini_llvm::writeAll(const char *path, const char *data, size_t size) {
    FileHandle handle(path, "w");
    if (!handle) {
        return Unexpected(errno);
    }
    if (fwrite(data, 1, size, handle.get()) != size) {
        return Unexpected(errno);
    }
    return {};
}
