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
    VTModeGuard(FILE */*stream*/, bool /*enableVTMode*/) {}
    ~VTModeGuard() = default;
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

    constexpr ColoredStringView(std::string_view str) noexcept
        : str(str) {}

    constexpr ColoredStringView(std::string_view str, Codes codes) noexcept
        : str(str), codes(codes) {}

    operator std::string() const;
};

struct ColoredString {
    using Codes = std::bitset<128>;

    std::string str;
    Codes codes;

    constexpr ColoredString(std::string str) noexcept
        : str(std::move(str)) {}

    constexpr ColoredString(std::string str, Codes codes) noexcept
        : str(std::move(str)), codes(codes) {}

    constexpr operator ColoredStringView() const noexcept {
        return ColoredStringView(str, codes);
    }

    operator std::string() const {
        return std::string(ColoredStringView(str, codes));
    }
};

class Color {
public:
    using Codes = std::bitset<128>;

    constexpr explicit Color(Codes codes) noexcept : codes_(codes) {}

    constexpr Color(std::initializer_list<int> il) noexcept {
        for (int code : il) {
            codes_.set(code);
        }
    }

    constexpr Codes codes() const noexcept {
        return codes_;
    }

    constexpr ColoredStringView operator()(ColoredStringView str) const noexcept {
        str.codes |= codes_;
        return str;
    }

    constexpr ColoredStringView operator()(std::string_view str) const noexcept {
        return (*this)(ColoredStringView(str));
    }

    template <typename T>
        requires std::same_as<T &&, ColoredString &&>
    constexpr ColoredString operator()(T &&str) const noexcept {
        str.codes |= codes_;
        // NOLINTNEXTLINE(bugprone-move-forwarding-reference)
        return std::move(str);
    }

    template <typename T>
        requires std::same_as<T &&, std::string &&>
    constexpr ColoredString operator()(T &&str) const noexcept {
        // NOLINTNEXTLINE(bugprone-move-forwarding-reference)
        return (*this)(ColoredString(std::move(str)));
    }

private:
    Codes codes_;
};

constexpr Color operator~(Color color) noexcept {
    return Color(~color.codes());
}

constexpr Color operator&(Color lhs, Color rhs) noexcept {
    return Color(lhs.codes() & rhs.codes());
}

constexpr Color operator|(Color lhs, Color rhs) noexcept {
    return Color(lhs.codes() | rhs.codes());
}

constexpr Color operator^(Color lhs, Color rhs) noexcept {
    return Color(lhs.codes() ^ rhs.codes());
}

constexpr Color &operator&=(Color &lhs, Color rhs) noexcept {
    lhs = lhs & rhs;
    return lhs;
}

constexpr Color &operator|=(Color &lhs, Color rhs) noexcept {
    lhs = lhs | rhs;
    return lhs;
}

constexpr Color &operator^=(Color &lhs, Color rhs) noexcept {
    lhs = lhs ^ rhs;
    return lhs;
}

namespace colors {

#define X(code, name) inline constexpr Color name{code};
#include "mini-llvm/utils/Color.def"
#undef X

} // namespace colors

} // namespace mini_llvm

template <>
struct std::formatter<mini_llvm::ColoredStringView> {
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
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
