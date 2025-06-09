#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/GlobalValue.h"

namespace mini_llvm::mir {

class PtrArrayConstant final : public Constant {
public:
    PtrArrayConstant(int ptrSize, std::vector<GlobalValue *> elements)
        : ptrSize_(ptrSize), elements_(std::move(elements)) {}

    int ptrSize() const {
        return ptrSize_;
    }

    const std::vector<GlobalValue *> &elements() const {
        return elements_;
    }

    void setElements(std::vector<GlobalValue *> elements) {
        elements_ = std::move(elements);
    }

    size_t numElements() const {
        return elements_.size();
    }

    int size() const override {
        return static_cast<int>(numElements()) * ptrSize_;
    }

    std::string format() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitPtrArrayConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitPtrArrayConstant(*this);
    }

private:
    int ptrSize_;
    std::vector<GlobalValue *> elements_;
};

} // namespace mini_llvm::mir
