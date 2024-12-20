#pragma once

#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/ScopeGuard.h"
#include "mini-llvm/utils/Status.h"
#include "mini-llvm/utils/SystemError.h"

namespace mini_llvm {

inline Expected<std::string, SystemError> readAll(const std::filesystem::path &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp) {
        return SystemError("fopen", errno);
    }
    ScopeGuard guard([fp] { fclose(fp); });
    if (fseek(fp, 0, SEEK_END) != -1) {
        long size = ftell(fp);
        if (size == -1) {
            return SystemError("ftell", errno);
        }
        if (fseek(fp, 0, SEEK_SET) == -1) {
            return SystemError("fseek", errno);
        }
        std::string content(size, '\0');
        size_t numRead = fread(content.data(), 1, size, fp);
        if (numRead != static_cast<size_t>(size)) {
            return SystemError("fread", errno);
        }
        return content;
    } else {
        std::string content;
        std::string chunk(4096, '\0');
        for (;;) {
            size_t numRead = fread(chunk.data(), 1, chunk.size(), fp);
            if (numRead < chunk.size() && ferror(fp)) {
                return SystemError("fread", errno);
            }
            content.append(chunk.data(), numRead);
            if (numRead < chunk.size()) {
                break;
            }
        }
        return content;
    }
}

inline Status<SystemError> writeAll(const std::filesystem::path &path, std::string_view content) {
    FILE *fp = fopen(path.c_str(), "w");
    if (!fp) {
        return SystemError("fopen", errno);
    }
    ScopeGuard guard([fp] { fclose(fp); });
    size_t numWritten = fwrite(content.data(), 1, content.size(), fp);
    if (numWritten != content.size()) {
        return SystemError("fwrite", errno);
    }
    return Ok();
}

} // namespace mini_llvm
