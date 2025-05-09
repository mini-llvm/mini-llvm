#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Expected.h"

namespace mini_llvm {

Expected<std::string, int> readAll(const std::filesystem::path &path);
Expected<void, int> writeAll(const std::filesystem::path &path, std::string_view content);

} // namespace mini_llvm
