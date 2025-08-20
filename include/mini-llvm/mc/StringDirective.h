// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT StringDirective final : public Directive {
public:
    explicit StringDirective(std::vector<int8_t> elements)
        : elements_(std::move(elements)) {}

    std::vector<int8_t> &elements() & {
        return elements_;
    }

    const std::vector<int8_t> &elements() const & {
        return elements_;
    }

    std::vector<int8_t> &&elements() && {
        return std::move(elements_);
    }

    void setElements(std::vector<int8_t> elements) {
        elements_ = std::move(elements);
    }

    std::string format() const override;

private:
    std::vector<int8_t> elements_;
};

} // namespace mini_llvm::mc
