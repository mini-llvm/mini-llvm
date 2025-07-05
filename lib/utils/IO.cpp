#include "mini-llvm/utils/IO.h"

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <string>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileHandle.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemError.h"

using namespace mini_llvm;

Expected<std::string, SystemError> mini_llvm::readAll(FILE *stream) {
    std::string content;
    std::string chunk(4096, '\0');
    for (;;) {
        size_t n = fread(chunk.data(), 1, chunk.size(), stream);
        content.append(chunk.data(), n);
        if (n < chunk.size()) {
            if (ferror(stream)) {
                return Unexpected(SystemError(errno));
            }
            break;
        }
    }
    return content;
}

Expected<std::string, SystemError> mini_llvm::readAll(const Path &path) {
    FileHandle handle(path, "rb");
    if (!handle) {
        return Unexpected(SystemError(errno));
    }
    return readAll(handle.get());
}

Expected<void, SystemError> mini_llvm::writeAll(FILE *stream, const char *data, size_t size) {
    if (fwrite(data, 1, size, stream) != size) {
        return Unexpected(SystemError(errno));
    }
    return {};
}

Expected<void, SystemError> mini_llvm::writeAll(const Path &path, const char *data, size_t size) {
    FileHandle handle(path, "wb");
    if (!handle) {
        return Unexpected(SystemError(errno));
    }
    return writeAll(handle.get(), data, size);
}
