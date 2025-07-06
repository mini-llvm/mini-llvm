#pragma once

#include <cstdint>
#include <string>

namespace mini_llvm {

std::string toString(int32_t value, int base = 10);
std::string toString(int64_t value, int base = 10);
std::string toString(uint32_t value, int base = 10);
std::string toString(uint64_t value, int base = 10);

void normalizeLineEndings(std::string &str);

} // namespace mini_llvm
