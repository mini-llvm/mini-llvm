// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/GetElementPtr.h"

#include <cstddef>
#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

GetElementPtr::GetElementPtr(std::unique_ptr<Type> sourceType,
                             std::shared_ptr<Value> ptr,
                             std::vector<std::shared_ptr<Value>> indices)
        : sourceType_(std::move(sourceType)), ptr_(this, std::move(ptr)) {
    for (auto &idx : indices) {
        indices_.push_back(std::make_unique<Use<Value>>(this, std::move(idx)));
    }
}

std::unordered_set<const UseBase *> GetElementPtr::operands() const {
    std::unordered_set<const UseBase *> operands;
    operands.insert(&ptr());
    for (const UseBase &idx : indices(*this)) {
        operands.insert(&idx);
    }
    return operands;
}

bool GetElementPtr::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (*sourceType() == Void() || *sourceType() == BasicBlockType()) {
        return false;
    }
    if (&*ptr() == this) {
        return false;
    }
    if (*ptr()->type() != Ptr()) {
        return false;
    }
    for (const Use<Value> &idx : indices(*this)) {
        if (&*idx == this) {
            return false;
        }
        if (!dynamic_cast<const IntegerType *>(&*idx->type())) {
            return false;
        }
    }
    size_t n = 1;
    std::unique_ptr<Type> type = sourceType();
    while (dynamic_cast<const ArrayType *>(&*type)) {
        ++n;
        type = static_cast<const ArrayType *>(&*type)->elementType();
    }
    if (idx_size() > n) {
        return false;
    }
    return true;
}

std::string GetElementPtr::format() const {
    StringJoiner formatted(", ");
    formatted.add("{:o} = getelementptr {}, {} {:o}", *this, *sourceType(), *ptr()->type(), *ptr());
    for (const Use<Value> &idx : indices(*this)) {
        formatted.add("{} {:o}", *idx->type(), *idx);
    }
    return formatted.toString();
}

std::unique_ptr<Value> GetElementPtr::clone() const {
    std::vector<std::shared_ptr<Value>> clonedIndices;
    for (const Use<Value> &idx : indices(*this)) {
        clonedIndices.push_back(share(*idx));
    }
    return std::make_unique<GetElementPtr>(sourceType(), share(*ptr()), std::move(clonedIndices));
}
