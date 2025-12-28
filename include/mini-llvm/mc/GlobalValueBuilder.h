// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Statement.h"

namespace mini_llvm::mc {

class GlobalValueBuilder {
public:
    GlobalValueBuilder() = default;

    GlobalValueBuilder(GlobalValue *globalValue, GlobalValue::const_iterator pos)
        : globalValue_(globalValue), pos_(pos) {}

    void setPos(GlobalValue *globalValue, GlobalValue::const_iterator pos) {
        globalValue_ = globalValue;
        pos_ = pos;
    }

    void setPos(GlobalValue *G) {
        setPos(G, G->end());
    }

    Statement &add(std::unique_ptr<Statement> stmt) {
        return globalValue_->add(pos_, std::move(stmt));
    }

private:
    GlobalValue *globalValue_{};
    GlobalValue::const_iterator pos_{};
};

} // namespace mini_llvm::mc
