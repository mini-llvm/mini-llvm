#pragma once

#include <exception>

namespace mini_llvm {

class PoisonValueException : public std::exception {
public:
    constexpr const char *what() const noexcept override {
        return "PoisonValueException";
    }
};

} // namespace mini_llvm
