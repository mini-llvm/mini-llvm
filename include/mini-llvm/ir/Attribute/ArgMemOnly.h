// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>

#include "mini-llvm/ir/Attribute.h"

namespace mini_llvm::ir {

class ArgMemOnly final : public Attribute {
public:
    std::string format() const override {
        return "argmemonly";
    }

    std::unique_ptr<Attribute> clone() const override {
        return std::make_unique<ArgMemOnly>();
    }
};

} // namespace mini_llvm::ir
