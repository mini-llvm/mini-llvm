#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/Status.h"
#include "mini-llvm/utils/SystemError.h"

namespace mini_llvm {

Expected<std::string, SystemError> readAll(const std::filesystem::path &path);
Status<SystemError> writeAll(const std::filesystem::path &path, std::string_view content);

} // namespace mini_llvm
