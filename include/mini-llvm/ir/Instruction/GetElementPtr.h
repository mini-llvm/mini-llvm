// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT GetElementPtr final : public Instruction {
    using IdxList = std::vector<std::unique_ptr<Use<Value>>>;

public:
    using idx_iterator = IndirectIterator<IdxList::iterator, Use<Value>>;
    using const_idx_iterator = IndirectIterator<IdxList::const_iterator, const Use<Value>>;

    GetElementPtr(std::unique_ptr<Type> sourceType, std::shared_ptr<Value> ptr, std::vector<std::shared_ptr<Value>> indices);

    std::unique_ptr<Type> sourceType() const {
        return sourceType_->clone();
    }

    template <typename Self>
    auto &ptr(this Self &&self) {
        return self.ptr_;
    }

    idx_iterator idx_begin() {
        return idx_iterator(indices_.begin());
    }

    const_idx_iterator idx_begin() const {
        return const_idx_iterator(indices_.begin());
    }

    idx_iterator idx_end() {
        return idx_iterator(indices_.end());
    }

    const_idx_iterator idx_end() const {
        return const_idx_iterator(indices_.end());
    }

    bool idx_empty() const {
        return indices_.empty();
    }

    size_t idx_size() const {
        return indices_.size();
    }

    Use<Value> &idx(size_t i) {
        return *indices_[i];
    }

    const Use<Value> &idx(size_t i) const {
        return *indices_[i];
    }

    std::unordered_set<const UseBase *> operands() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitGetElementPtr(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitGetElementPtr(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    std::unique_ptr<Type> sourceType_;
    Use<Value> ptr_;
    IdxList indices_;
};

inline auto indices(GetElementPtr &I) {
    return std::ranges::subrange(I.idx_begin(), I.idx_end());
}

inline auto indices(const GetElementPtr &I) {
    return std::ranges::subrange(I.idx_begin(), I.idx_end());
}

} // namespace mini_llvm::ir
