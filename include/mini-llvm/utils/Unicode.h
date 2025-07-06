#pragma once

#include <optional>

namespace mini_llvm {

std::optional<char32_t> decodeUtf8(const char *&ptr) noexcept;

} // namespace mini_llvm
