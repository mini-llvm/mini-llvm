// SPDX-License-Identifier: MIT

#pragma once

#include <bitset>
#include <concepts>
#include <cstdio>
#include <format>
#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

MINI_LLVM_EXPORT bool supportsColor(FILE *stream);

class
#ifdef _WIN32
    MINI_LLVM_EXPORT
#endif
VTModeGuard {
public:
#ifdef _WIN32
    VTModeGuard(FILE *stream, bool enableVTMode);
    ~VTModeGuard();
#else
    VTModeGuard(FILE *, bool) {}
#endif

    VTModeGuard(const VTModeGuard &) = delete;
    VTModeGuard &operator=(const VTModeGuard &) = delete;

    VTModeGuard(VTModeGuard &&) = delete;
    VTModeGuard &operator=(VTModeGuard &&) = delete;

private:
#ifdef _WIN32
    void *hConsole_;
    bool oldVTModeEnabled_;
#endif
};

class MINI_LLVM_EXPORT ColorGuard {
public:
    explicit ColorGuard(bool enableColor);
    ~ColorGuard();

    ColorGuard(const ColorGuard &) = delete;
    ColorGuard &operator=(const ColorGuard &) = delete;

    ColorGuard(ColorGuard &&) = delete;
    ColorGuard &operator=(ColorGuard &&) = delete;

private:
    bool oldColorEnabled_;
};

struct MINI_LLVM_EXPORT ColoredStringView {
    using Codes = std::bitset<128>;

    std::string_view str;
    Codes codes;

    ColoredStringView(std::string_view str)
        : str(str) {}

    ColoredStringView(std::string_view str, Codes codes)
        : str(str), codes(codes) {}

    operator std::string() const;
};

struct ColoredString {
    using Codes = std::bitset<128>;

    std::string str;
    Codes codes;

    ColoredString(std::string str)
        : str(std::move(str)) {}

    ColoredString(std::string str, Codes codes)
        : str(std::move(str)), codes(codes) {}

    operator ColoredStringView() const {
        return ColoredStringView(str, codes);
    }

    operator std::string() const {
        return std::string(ColoredStringView(str, codes));
    }
};

class Color {
public:
    using Codes = std::bitset<128>;

    explicit Color(Codes codes) : codes_(codes) {}

    Color(std::initializer_list<int> il) {
        for (int code : il) {
            codes_.set(code);
        }
    }

    Codes codes() const {
        return codes_;
    }

    ColoredStringView operator()(ColoredStringView str) const {
        str.codes |= codes_;
        return str;
    }

    ColoredStringView operator()(std::string_view str) const {
        return (*this)(ColoredStringView(str));
    }

    template <typename T>
        requires std::same_as<T &&, ColoredString &&>
    ColoredString operator()(T &&str) const {
        str.codes |= codes_;
        return str;
    }

    template <typename T>
        requires std::same_as<T &&, std::string &&>
    ColoredString operator()(T &&str) const {
        return (*this)(ColoredString(std::move(str)));
    }

private:
    Codes codes_;
};

inline Color operator~(Color color) {
    return Color(~color.codes());
}

inline Color operator&(Color lhs, Color rhs) {
    return Color(lhs.codes() & rhs.codes());
}

inline Color operator|(Color lhs, Color rhs) {
    return Color(lhs.codes() | rhs.codes());
}

inline Color operator^(Color lhs, Color rhs) {
    return Color(lhs.codes() ^ rhs.codes());
}

inline Color &operator&=(Color &lhs, Color rhs) {
    lhs = lhs & rhs;
    return lhs;
}

inline Color &operator|=(Color &lhs, Color rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline Color &operator^=(Color &lhs, Color rhs) {
    lhs = lhs ^ rhs;
    return lhs;
}

namespace colors {

#define X(code, name) inline const Color name{code};
#include "mini-llvm/utils/Color.def"
#undef X

} // namespace colors

} // namespace mini_llvm

template <>
struct std::formatter<mini_llvm::ColoredStringView> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::ColoredStringView &str, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", std::string(str));
    }
};

template <>
struct std::formatter<mini_llvm::ColoredString>
    : std::formatter<mini_llvm::ColoredStringView> {};
