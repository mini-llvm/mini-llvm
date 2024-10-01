#pragma once

#include <cassert>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/mir/Constant.h"

namespace mini_llvm::mir {

class GlobalVar {
public:
    GlobalVar(std::string name, Linkage linkage, std::optional<std::unique_ptr<Constant>> initializer = std::nullopt)
        : name_(std::move(name)), linkage_(linkage), initializer_(std::move(initializer)) {}

    GlobalVar(const GlobalVar &&) = delete;
    GlobalVar(GlobalVar &&) = delete;
    GlobalVar &operator=(const GlobalVar &&) = delete;
    GlobalVar &operator=(GlobalVar &&) = delete;

    const std::string &name() const {
        return name_;
    }

    Linkage linkage() const {
        return linkage_;
    }

    bool hasInitializer() const {
        return initializer_.has_value();
    }

    Constant &initializer() {
        return *initializer_.value();
    }

    const Constant &initializer() const {
        return *initializer_.value();
    }

    void setInitializer(std::unique_ptr<Constant> data) {
        initializer_ = std::move(data);
    }

    std::string format() const;

    std::string formatAsOperand() const {
        return "@" + name();
    }

private:
    std::string name_;
    Linkage linkage_;
    std::optional<std::unique_ptr<Constant>> initializer_;
};

} // namespace mini_llvm::mir

template <>
struct std::formatter<mini_llvm::mir::GlobalVar> {
    constexpr auto parse(std::format_parse_context &ctx) {
        if (*ctx.begin() == 'o') {
            asOperand_ = true;
            return std::next(ctx.begin());
        }
        return ctx.begin();
    }

    auto format(const mini_llvm::mir::GlobalVar &G, std::format_context &ctx) const {
        if (asOperand_) {
            return std::format_to(ctx.out(), "{}", G.formatAsOperand());
        }
        return std::format_to(ctx.out(), "{}", G.format());
    }

private:
    bool asOperand_ = false;
};
