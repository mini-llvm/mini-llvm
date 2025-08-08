// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>

#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/StackSlot.h"

namespace mini_llvm::mir {

class StackOffsetImmediate final : public Immediate {
public:
    StackOffsetImmediate(StackSlot *baseSlot, StackSlot *slot, int displacement = 0)
        : baseSlot_(baseSlot), slot_(slot), displacement_(displacement) {}

    StackSlot *baseSlot() const {
        return baseSlot_;
    }

    StackSlot *slot() const {
        return slot_;
    }

    int displacement() const {
        return displacement_;
    }

    int64_t value() const override {
        return slot()->offset() - baseSlot()->offset() + displacement();
    }

    std::unique_ptr<Immediate> clone() const override {
        return std::make_unique<StackOffsetImmediate>(baseSlot(), slot(), displacement());
    }

private:
    StackSlot *baseSlot_, *slot_;
    int displacement_;
};

} // namespace mini_llvm::mir
