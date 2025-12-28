// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>

#include "mini-llvm/ir/Attribute.h"

namespace mini_llvm::ir {

class AlwaysInline final : public Attribute {
public:
    std::string format() const override {
        return "alwaysinline";
    }

    std::unique_ptr<Attribute> clone() const override {
        return std::make_unique<AlwaysInline>();
    }
};

} // namespace mini_llvm::ir
