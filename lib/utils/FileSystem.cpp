#include "mini-llvm/utils/FileSystem.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/ScopeGuard.h"
#include "mini-llvm/utils/Status.h"
#include "mini-llvm/utils/SystemError.h"

using namespace mini_llvm;

namespace {

Expected<std::string, SystemError> readAllSeekable(const std::filesystem::path &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp) {
        return SystemError{"fopen", errno};
    }
    ScopeGuard guard(fclose, fp);
    if (fseek(fp, 0, SEEK_END) == -1) {
        return SystemError{"fseek", errno};
    }
    long size = ftell(fp);
    if (size == -1) {
        return SystemError{"ftell", errno};
    }
    if (fseek(fp, 0, SEEK_SET) == -1) {
        return SystemError{"fseek", errno};
    }
    std::string content(size, '\0');
    size_t numRead = fread(content.data(), 1, size, fp);
    if (numRead != (size_t)size) {
        return SystemError{"fread", errno};
    }
    return content;
}

Expected<std::string, SystemError> readAllNonSeekable(const std::filesystem::path &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp) {
        return SystemError{"fopen", errno};
    }
    ScopeGuard guard(fclose, fp);
    std::string content;
    std::string chunk(4096, '\0');
    for (;;) {
        size_t numRead = fread(chunk.data(), 1, chunk.size(), fp);
        content.append(chunk.data(), numRead);
        if (numRead < chunk.size()) {
            if (ferror(fp)) {
                return SystemError{"fread", errno};
            }
            break;
        }
    }
    return content;
}

} // namespace

Expected<std::string, SystemError> mini_llvm::readAll(const std::filesystem::path &path) {
    Expected<std::string, SystemError> content = readAllSeekable(path);
    if (content || strcmp(content.error().call, "fopen") == 0) {
        return content;
    }
    content = readAllNonSeekable(path);
    return content;
}

Status<SystemError> mini_llvm::writeAll(const std::filesystem::path &path, std::string_view content) {
    FILE *fp = fopen(path.c_str(), "w");
    if (!fp) {
        return SystemError{"fopen", errno};
    }
    ScopeGuard guard(fclose, fp);
    size_t numWritten = fwrite(content.data(), 1, content.size(), fp);
    if (numWritten != content.size()) {
        return SystemError{"fwrite", errno};
    }
    return Ok();
}
