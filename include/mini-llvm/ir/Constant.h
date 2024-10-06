#pragma once

#include <string>

#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class Constant : public Value {
public:
    virtual void accept(ConstantVisitor &visitor) = 0;
    virtual void accept(ConstantVisitor &visitor) const = 0;

    std::string formatAsOperand() const override {
        return format();
    }

protected:
    virtual bool equals(const Constant &other) const = 0;

    friend bool operator==(const Constant &lhs, const Constant &rhs);
};

inline bool operator==(const Constant &lhs, const Constant &rhs) {
    return lhs.equals(rhs);
}

} // namespace mini_llvm::ir
