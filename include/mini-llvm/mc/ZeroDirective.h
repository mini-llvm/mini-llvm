#pragma once

#include <format>
#include <string>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT ZeroDirective : public Directive {
public:
    explicit ZeroDirective(int size) : size_(size) {}

    int size() const {
        return size_;
    }

    void setSize(int size) {
        size_ = size;
    }

    std::string format() const override {
        return std::format(".zero {}", size());
    }

private:
    int size_;
};

} // namespace mini_llvm::mc
