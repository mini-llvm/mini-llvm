#pragma once

#include <cstdint>
#include <memory>

#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/StackSlot.h"

namespace mini_llvm::mir {

class StackRelativeOffsetImmediate final : public Immediate {
public:
    StackRelativeOffsetImmediate(StackSlot *slot1, StackSlot *slot2, int displacement = 0)
        : slot1_(slot1), slot2_(slot2), displacement_(displacement) {}

    StackSlot *slot1() const {
        return slot1_;
    }

    StackSlot *slot2() const {
        return slot2_;
    }

    int displacement() const {
        return displacement_;
    }

    int64_t value() const override {
        return slot2()->offset() - slot1()->offset() + displacement();
    }

    std::unique_ptr<Immediate> clone() const override {
        return std::make_unique<StackRelativeOffsetImmediate>(slot1(), slot2(), displacement());
    }

private:
    StackSlot *slot1_, *slot2_;
    int displacement_;
};

} // namespace mini_llvm::mir
