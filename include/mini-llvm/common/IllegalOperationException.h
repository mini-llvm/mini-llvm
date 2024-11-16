#pragma once

#include <exception>

namespace mini_llvm {

class IllegalOperationException : public std::exception {
public:
    constexpr const char *what() const noexcept override {
        return "IllegalOperationException";
    }
};

} // namespace mini_llvm
