#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <typeinfo>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Use.h"

namespace mini_llvm::ir {

class GlobalVar final : public GlobalValue {
public:
    GlobalVar(std::unique_ptr<Type> valueType,
              Linkage linkage,
              bool isConstant,
              std::optional<std::shared_ptr<Constant>> initializer = std::nullopt);

    std::unique_ptr<Type> valueType() const {
        return valueType_->clone();
    }

    Linkage linkage() const {
        return linkage_;
    }

    bool isConstant() const {
        return isConstant_;
    }

    Constant &initializer() const {
        return **initializer_;
    }

    void setInitializer(std::optional<std::shared_ptr<Constant>> initializer);

    bool isDeclaration() const override {
        return !initializer_;
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitGlobalVar(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitGlobalVar(*this);
    }

    std::string format() const override;

protected:
    bool equals(const Constant &other) const override {
        assert(typeid(*this) != typeid(other));
        return false;
    }

private:
    std::unique_ptr<Type> valueType_;
    Linkage linkage_;
    bool isConstant_;
    std::optional<Use<Constant>> initializer_;
};

} // namespace mini_llvm::ir
