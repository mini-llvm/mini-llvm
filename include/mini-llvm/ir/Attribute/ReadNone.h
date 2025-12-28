// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>

#include "mini-llvm/ir/Attribute.h"

namespace mini_llvm::ir {

class ReadNone final : public Attribute {
public:
    std::string format() const override {
        return "readnone";
    }

    std::unique_ptr<Attribute> clone() const override {
        return std::make_unique<ReadNone>();
    }
};

} // namespace mini_llvm::ir
