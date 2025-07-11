#pragma once

#include <compare>
#include <format>
#include <functional>
#include <string>

namespace mini_llvm::mc {

class Symbol {
public:
    explicit Symbol(std::string name) : name_(std::move(name)) {}

    const std::string &name() const {
        return name_;
    }

    std::string format() const;

private:
    std::string name_;
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
    return lhs.name() == rhs.name();
}

inline std::strong_ordering operator<=>(const Symbol &lhs, const Symbol &rhs) {
    return lhs.name() <=> rhs.name();
}

} // namespace mini_llvm::mc

template <>
struct std::hash<mini_llvm::mc::Symbol> {
    size_t operator()(const mini_llvm::mc::Symbol &symbol) const {
        return std::hash<std::string>()(symbol.name());
    }
};

template <>
struct std::formatter<mini_llvm::mc::Symbol> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const mini_llvm::mc::Symbol &symbol, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", symbol.format());
    }
};
