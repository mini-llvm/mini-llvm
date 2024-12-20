#include "mini-llvm/utils/FileSystem.h"

#include <cstddef>
#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/ScopeGuard.h"
#include "mini-llvm/utils/Status.h"
#include "mini-llvm/utils/SystemError.h"

using namespace mini_llvm;

static constexpr size_t kChunkSize = 4096;

Expected<std::string, SystemError> mini_llvm::readAll(const std::filesystem::path &path) {
    FILE *fp = fopen(path.c_str(), "r");
    if (!fp) {
        return SystemError("fopen", errno);
    }
    ScopeGuard guard([fp] { fclose(fp); });
    std::string content;
    if (fseek(fp, 0, SEEK_END) != -1) {
        long size = ftell(fp);
        if (size == -1) {
            return SystemError("ftell", errno);
        }
        if (fseek(fp, 0, SEEK_SET) == -1) {
            return SystemError("fseek", errno);
        }
        content.resize(size);
        size_t numRead = fread(content.data(), 1, size, fp);
        if (numRead != static_cast<size_t>(size)) {
            return SystemError("fread", errno);
        }
    } else {
        std::string chunk(kChunkSize, '\0');
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
    }
    return content;
}

Status<SystemError> mini_llvm::writeAll(const std::filesystem::path &path, std::string_view content) {
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
