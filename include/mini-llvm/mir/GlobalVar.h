#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/GlobalValue.h"

namespace mini_llvm::mir {

class GlobalVar final : public GlobalValue {
public:
    GlobalVar(std::string name,
              Linkage linkage,
              bool isConstant,
              std::optional<std::unique_ptr<Constant>> initializer = std::nullopt)
        : name_(std::move(name)),
          linkage_(linkage),
          isConstant_(isConstant),
          initializer_(std::move(initializer)) {}

    GlobalVar(const GlobalVar &&) = delete;
    GlobalVar(GlobalVar &&) = delete;
    GlobalVar &operator=(const GlobalVar &&) = delete;
    GlobalVar &operator=(GlobalVar &&) = delete;

    std::string name() const override {
        return name_;
    }

    Linkage linkage() const override {
        return linkage_;
    }

    bool isConstant() const {
        return isConstant_;
    }

    bool isDeclaration() const {
        return !initializer_;
    }

    Constant &initializer() {
        return **initializer_;
    }

    const Constant &initializer() const {
        return **initializer_;
    }

    void setInitializer(std::unique_ptr<Constant> data) {
        initializer_ = std::move(data);
    }

    std::string format() const override;

private:
    std::string name_;
    Linkage linkage_;
    bool isConstant_;
    std::optional<std::unique_ptr<Constant>> initializer_;
};

} // namespace mini_llvm::mir
