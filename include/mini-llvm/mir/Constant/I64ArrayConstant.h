#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"

namespace mini_llvm::mir {

class I64ArrayConstant final : public Constant {
public:
    explicit I64ArrayConstant(std::vector<int64_t> elements) : elements_(std::move(elements)) {}

    std::vector<int64_t> &elements() {
        return elements_;
    }

    const std::vector<int64_t> &elements() const {
        return elements_;
    }

    void setElements(std::vector<int64_t> elements) {
        elements_ = std::move(elements);
    }

    size_t numElements() const {
        return elements_.size();
    }

    int size() const override {
        return static_cast<int>(numElements()) * 4;
    }

    std::string format() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI64ArrayConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI64ArrayConstant(*this);
    }

private:
    std::vector<int64_t> elements_;
};

} // namespace mini_llvm::mir
