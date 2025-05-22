#pragma once

#include <cstdio>
#include <format>
#include <print>
#include <regex>
#include <string>
#include <vector>

namespace mini_llvm {

enum class Color {
    kReset = 0,
    kBold = 1,
    kDim = 2,
    kItalic = 3,
    kUnderline = 4,
    kInverse = 7,
    kStrikethrough = 9,
    kBlack = 30,
    kRed = 31,
    kGreen = 32,
    kYellow = 33,
    kBlue = 34,
    kMagenta = 35,
    kCyan = 36,
    kWhite = 37,
    kBrightBlack = 90,
    kBrightRed = 91,
    kBrightGreen = 92,
    kBrightYellow = 93,
    kBrightBlue = 94,
    kBrightMagenta = 95,
    kBrightCyan = 96,
    kBrightWhite = 97,
    kOnBlack = 40,
    kOnRed = 41,
    kOnGreen = 42,
    kOnYellow = 43,
    kOnBlue = 44,
    kOnMagenta = 45,
    kOnCyan = 46,
    kOnWhite = 47,
    kOnBrightBlack = 100,
    kOnBrightRed = 101,
    kOnBrightGreen = 102,
    kOnBrightYellow = 103,
    kOnBrightBlue = 104,
    kOnBrightMagenta = 105,
    kOnBrightCyan = 106,
    kOnBrightWhite = 107,
};

class Colors : public std::vector<Color> {
public:
    Colors() = default;

    Colors(Color color) {
        push_back(color);
    }

    Colors &operator=(Color color) {
        clear();
        push_back(color);
        return *this;
    }

    Colors &operator+=(Color color) {
        push_back(color);
        return *this;
    }

    Colors &operator+=(const Colors &colors) {
        insert(end(), colors.begin(), colors.end());
        return *this;
    }
};

inline Colors operator+(Colors lhs, Color rhs) {
    return lhs += rhs;
}

inline Colors operator+(Colors lhs, const Colors &rhs) {
    return lhs += rhs;
}

inline Colors operator+(Color lhs, Color rhs) {
    return Colors(lhs) + rhs;
}

inline Colors operator+(Color lhs, const Colors &rhs) {
    return Colors(lhs) + rhs;
}

inline std::string removeColor(const std::string &str) {
    static std::regex regex(R"(\x1b\[[0-9;]*m)");
    return std::regex_replace(str, regex, "");
}

template <typename... Args>
std::string format(bool withColor, std::format_string<Args...> fmt, Args &&...args) {
    std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
    if (!withColor) {
        str = removeColor(str);
    }
    return str;
}

template <typename... Args>
void print(bool withColor, std::format_string<Args...> fmt, Args &&...args) {
    std::print("{}", format(withColor, fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void print(FILE *stream, bool withColor, std::format_string<Args...> fmt, Args &&...args) {
    std::print(stream, "{}", format(withColor, fmt, std::forward<Args>(args)...));
}

bool supportsColor(FILE *stream);

} // namespace mini_llvm

template <>
struct std::formatter<mini_llvm::Color> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(mini_llvm::Color color, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "\x1b[{}m", static_cast<int>(color));
    }
};

template <>
struct std::formatter<mini_llvm::Colors> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::Colors &colors, FormatContext &ctx) const {
        auto out = ctx.out();
        out = std::format_to(out, "\x1b[");
        auto i = colors.begin(), e = colors.end();
        if (i != e) {
            out = std::format_to(out, "{}", static_cast<int>(*i));
            ++i;
            while (i != e) {
                out = std::format_to(out, ";{}", static_cast<int>(*i));
                ++i;
            }
        }
        out = std::format_to(out, "m");
        return out;
    }
};
