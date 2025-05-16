#pragma once

#include <cstdio>
#include <format>
#include <print>
#include <string>
#include <vector>

namespace mini_llvm {

enum class Color {
    kReset           = 0,
    kBold            = 1,
    kDim             = 2,
    kItalic          = 3,
    kUnderline       = 4,
    kInverse         = 7,
    kStrikethrough   = 9,
    kBlack           = 30,
    kRed             = 31,
    kGreen           = 32,
    kYellow          = 33,
    kBlue            = 34,
    kMagenta         = 35,
    kCyan            = 36,
    kWhite           = 37,
    kBrightBlack     = 90,
    kBrightRed       = 91,
    kBrightGreen     = 92,
    kBrightYellow    = 93,
    kBrightBlue      = 94,
    kBrightMagenta   = 95,
    kBrightCyan      = 96,
    kBrightWhite     = 97,
    kOnBlack         = 40,
    kOnRed           = 41,
    kOnGreen         = 42,
    kOnYellow        = 43,
    kOnBlue          = 44,
    kOnMagenta       = 45,
    kOnCyan          = 46,
    kOnWhite         = 47,
    kOnBrightBlack   = 100,
    kOnBrightRed     = 101,
    kOnBrightGreen   = 102,
    kOnBrightYellow  = 103,
    kOnBrightBlue    = 104,
    kOnBrightMagenta = 105,
    kOnBrightCyan    = 106,
    kOnBrightWhite   = 107,
};

class Colors : public std::vector<Color> {
public:
    Colors() = default;

    Colors(Color color) {
        push_back(color);
    }
};

inline Colors operator+(Color lhs, Color rhs) {
    Colors colors;
    colors.push_back(lhs);
    colors.push_back(rhs);
    return colors;
}

inline Colors operator+(Colors lhs, Color rhs) {
    lhs.insert(lhs.end(), rhs);
    return lhs;
}

inline Colors operator+(Color lhs, Colors rhs) {
    rhs.insert(rhs.begin(), lhs);
    return rhs;
}

inline Colors operator+(Colors lhs, const Colors &rhs) {
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
}

inline void removeColor(std::string &str) {
    size_t i = 0;
    while ((i = str.find("\033[", i)) != std::string::npos) {
        size_t j = str.find('m', i);
        if (j != std::string::npos) {
            str.erase(i, j - i + 1);
        }
    }
}

template <typename... Args>
std::string format(bool withColor, std::format_string<Args...> fmt, Args &&...args) {
    std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
    if (!withColor) {
        removeColor(str);
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
        return std::format_to(ctx.out(), "\033[{}m", static_cast<int>(color));
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
        out = std::format_to(out, "\033[");
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
