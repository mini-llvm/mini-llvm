#pragma once

#include <exception>

namespace mini_llvm {

class OpException : public std::exception {
public:
    constexpr const char *what() const noexcept override {
        return "OpException";
    }
};

} // namespace mini_llvm
