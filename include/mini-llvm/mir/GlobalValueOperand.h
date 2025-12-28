// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/GlobalValue.h"

namespace mini_llvm::mir {

class GlobalValueOperand {
public:
    explicit GlobalValueOperand(GlobalValue *globalValue) : globalValue_(globalValue) {}

    GlobalValue &operator*() const {
        return *globalValue_;
    }

    GlobalValue *operator->() const {
        return globalValue_;
    }

    void set(GlobalValue *globalValue) {
        globalValue_ = globalValue;
    }

private:
    GlobalValue *globalValue_;
};

} // namespace mini_llvm::mir
