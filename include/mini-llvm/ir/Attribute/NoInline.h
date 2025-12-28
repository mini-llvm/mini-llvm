// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>

#include "mini-llvm/ir/Attribute.h"

namespace mini_llvm::ir {

class NoInline final : public Attribute {
public:
    std::string format() const override {
        return "noinline";
    }

    std::unique_ptr<Attribute> clone() const override {
        return std::make_unique<NoInline>();
    }
};

} // namespace mini_llvm::ir
