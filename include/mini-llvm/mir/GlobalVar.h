// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT GlobalVar final : public GlobalValue {
public:
    GlobalVar(std::string name,
              Linkage linkage,
              bool isConstant,
              int alignment,
              std::unique_ptr<Constant> initializer)
        : name_(std::move(name)),
          linkage_(linkage),
          isConstant_(isConstant),
          alignment_(alignment),
          initializer_(std::move(initializer)) {}

    GlobalVar(std::string name, Linkage linkage)
        : name_(std::move(name)),
          linkage_(linkage),
          isConstant_(false),
          alignment_(0),
          initializer_(nullptr) {}

    GlobalVar(const GlobalVar &&) = delete;
    GlobalVar(GlobalVar &&) = delete;
    GlobalVar &operator=(const GlobalVar &&) = delete;
    GlobalVar &operator=(GlobalVar &&) = delete;

    std::string name() const override {
        return name_;
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    Linkage linkage() const override {
        return linkage_;
    }

    void setLinkage(Linkage linkage) {
        linkage_ = linkage;
    }

    bool isConstant() const {
        return isConstant_;
    }

    void setConstant(bool isConstant) {
        isConstant_ = isConstant;
    }

    int alignment() const {
        return alignment_;
    }

    void setAlignment(int alignment) {
        alignment_ = alignment;
    }

    bool isDeclaration() const {
        return !initializer_;
    }

    Constant &initializer() {
        return *initializer_;
    }

    const Constant &initializer() const {
        return *initializer_;
    }

    void setInitializer(std::unique_ptr<Constant> initializer) {
        initializer_ = std::move(initializer);
    }

    std::string format() const override;

private:
    std::string name_;
    Linkage linkage_;
    bool isConstant_;
    int alignment_;
    std::unique_ptr<Constant> initializer_;
};

} // namespace mini_llvm::mir
