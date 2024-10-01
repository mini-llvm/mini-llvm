#pragma once

#include <memory>
#include <optional>

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
              std::optional<std::shared_ptr<Constant>> initializer = std::nullopt);

    std::unique_ptr<Type> valueType() const {
        return valueType_->clone();
    }

    Linkage linkage() const {
        return linkage_;
    }

    bool hasInitializer() const {
        return initializer_.has_value();
    }

    Constant &initializer() const {
        return *initializer_.value();
    }

    void setInitializer(std::optional<std::shared_ptr<Constant>> initializer);

    void accept(ConstantVisitor &visitor) override {
        visitor.visitGlobalVar(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitGlobalVar(*this);
    }

    std::string format() const override;

private:
    std::unique_ptr<Type> valueType_;
    Linkage linkage_;
    std::optional<Use<Constant>> initializer_;
};

} // namespace mini_llvm::ir
