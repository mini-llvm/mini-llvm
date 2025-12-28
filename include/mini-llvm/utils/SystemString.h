// SPDX-License-Identifier: MIT

#pragma once

#include <compare>
#include <cstddef>
#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#ifdef _WIN32
    #include "mini-llvm/utils/Windows.h"
#endif

namespace mini_llvm {

class SystemString {
public:
#ifdef _WIN32
    using value_type = wchar_t;
    using string_type = std::wstring;
#else
    using value_type = char;
    using string_type = std::string;
#endif

    SystemString() = default;

    SystemString(const char *str)
#ifdef _WIN32
        : str_(windows::widen(str)) {}
#else
        : str_(str) {}
#endif

    SystemString(const std::string &str)
#ifdef _WIN32
        : str_(windows::widen(str)) {}
#else
        : str_(str) {}
#endif

    SystemString(std::string &&str)
#ifdef _WIN32
        : str_(windows::widen(str)) {}
#else
        : str_(std::move(str)) {}
#endif

    SystemString(std::string_view str)
#ifdef _WIN32
        : str_(windows::widen(str)) {}
#else
        : str_(str) {}
#endif

#ifdef _WIN32
    SystemString(const wchar_t *str)
        : str_(str) {}

    SystemString(const std::wstring &str)
        : str_(str) {}

    SystemString(std::wstring &&str)
        : str_(std::move(str)) {}

    SystemString(std::wstring_view str)
        : str_(str) {}
#endif

    SystemString(const SystemString &other) = default;
    SystemString(SystemString &&other) noexcept = default;

    SystemString &operator=(SystemString other) noexcept {
        swap(other);
        return *this;
    }

    string_type &native() & noexcept {
        return str_;
    }

    const string_type &native() const & noexcept {
        return str_;
    }

    string_type &&native() && noexcept {
        return std::move(str_);
    }

    const value_type *c_str() const noexcept {
        return str_.c_str();
    }

    std::string to_string() const & {
#ifdef _WIN32
        return windows::narrow(str_);
#else
        return str_;
#endif
    }

    std::string to_string() && {
#ifdef _WIN32
        return windows::narrow(str_);
#else
        return std::move(str_);
#endif
    }

    void swap(SystemString &other) noexcept {
        str_.swap(other.str_);
    }

private:
    string_type str_;
};

inline void swap(SystemString &lhs, SystemString &rhs) noexcept {
    lhs.swap(rhs);
}

inline bool operator==(const SystemString &lhs, const SystemString &rhs) noexcept {
    return lhs.native() == rhs.native();
}

inline std::strong_ordering operator<=>(const SystemString &lhs, const SystemString &rhs) noexcept {
    return lhs.native() <=> rhs.native();
}

} // namespace mini_llvm

template <>
struct std::hash<mini_llvm::SystemString> {
    size_t operator()(const mini_llvm::SystemString &str) const noexcept {
        return std::hash<mini_llvm::SystemString::string_type>()(str.native());
    }
};

template <>
struct std::formatter<mini_llvm::SystemString> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::SystemString &str, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", str.to_string());
    }
};
