#pragma once

#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/mc/Directive.h"

namespace mini_llvm::mc {

class DataDirective : public Directive {
public:
    int width() const {
        return width_;
    }

    void setWidth(int width) {
        width_ = width;
    }

    int64_t value() const {
        return value_;
    }

    void setValue(int64_t value) {
        value_ = value;
    }

    std::string format() const override {
        return std::format(".{} {}", directiveName(width()), value());
    }

protected:
    DataDirective(int width, int64_t value) : width_(width), value_(value) {}

    virtual std::string directiveName(int width) const = 0;

private:
    int width_;
    int64_t value_;
};

} // namespace mini_llvm::mc
