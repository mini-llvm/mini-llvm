#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class Load final : public Instruction {
public:
    Load(std::unique_ptr<Type> type, std::shared_ptr<Value> addr)
        : type_(std::move(type)), ptr_(this, std::move(addr)) {}

    template <typename Self>
    auto &ptr(this Self &&self) {
        return self.ptr_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&ptr()};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLoad(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLoad(*this);
    }

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override {
        return std::format("{:o} = load {}, {} {:o}", *this, *type(), *ptr()->type(), *ptr());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<Load>(type(), share(*ptr()));
    }

private:
    std::unique_ptr<Type> type_;
    Use<Value, Ptr> ptr_;
};

} // namespace mini_llvm::ir
