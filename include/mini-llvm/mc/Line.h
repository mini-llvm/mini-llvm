#pragma once

#include <concepts>
#include <format>
#include <string>

namespace mini_llvm::mc {

class Line {
public:
    virtual ~Line() = default;
    Line() = default;
    Line(const Line &) = delete;
    Line(Line &&) = delete;
    Line &operator=(const Line &) = delete;
    Line &operator=(Line &&) = delete;

    virtual std::string format() const = 0;
};

} // namespace mini_llvm::mc

template <typename LineT>
    requires std::derived_from<LineT, mini_llvm::mc::Line>
struct std::formatter<LineT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const LineT &line, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", line.format());
    }
};
