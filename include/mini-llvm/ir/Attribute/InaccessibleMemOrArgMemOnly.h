// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>

#include "mini-llvm/ir/Attribute.h"

namespace mini_llvm::ir {

class InaccessibleMemOrArgMemOnly final : public Attribute {
public:
    std::string format() const override {
        return "inaccessiblemem_or_argmemonly";
    }

    std::unique_ptr<Attribute> clone() const override {
        return std::make_unique<InaccessibleMemOrArgMemOnly>();
    }
};

} // namespace mini_llvm::ir
