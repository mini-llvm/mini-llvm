// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <typeinfo>
#include <variant>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT GlobalVar final : public GlobalValue {
public:
    GlobalVar(std::unique_ptr<Type> valueType,
              Linkage linkage,
              bool isConstant,
              std::shared_ptr<Constant> initializer);

    GlobalVar(std::unique_ptr<Type> valueType, Linkage linkage);

    std::unique_ptr<Type> valueType() const {
        return valueType_->clone();
    }

    Linkage linkage() const {
        return linkage_;
    }

    bool isConstant() const {
        return isConstant_;
    }

    Constant &initializer();
    const Constant &initializer() const;

    void setInitializer(std::shared_ptr<Constant> initializer);

    bool isDeclaration() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitGlobalVar(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitGlobalVar(*this);
    }

    std::string format() const override;

protected:
    bool equals([[maybe_unused]] const Constant &other) const override {
        assert(typeid(*this) != typeid(other));
        return false;
    }

private:
    std::unique_ptr<Type> valueType_;
    Linkage linkage_;
    bool isConstant_;
    std::variant<std::monostate, std::shared_ptr<Constant>, std::weak_ptr<Constant>> initializer_;
};

} // namespace mini_llvm::ir
