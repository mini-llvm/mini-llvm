// SPDX-License-Identifier: MIT

#pragma once

#include <compare>
#include <cstddef>
#include <filesystem>
#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include "mini-llvm/utils/SystemString.h"

namespace mini_llvm {

class Path : private std::filesystem::path {
    using Base = std::filesystem::path;

public:
    static constexpr char preferred_separator = static_cast<char>(Base::preferred_separator);

    Path() = default;

    Path(const Base &base)
        : Base(base) {}

    Path(Base &&base) noexcept
        : Base(std::move(base)) {}

    Path(const SystemString &str)
        : Base(str.native()) {}

    Path(SystemString &&str)
        : Base(std::move(str).native()) {}

    Path(const char *str)
        : Path(SystemString(str)) {}

    Path(const std::string &str)
        : Path(SystemString(str)) {}

    Path(std::string &&str)
        : Path(SystemString(std::move(str))) {}

    Path(std::string_view str)
        : Path(SystemString(str)) {}

    Path(const Path &other) = default;
    Path(Path &&other) noexcept = default;

    Path &operator=(const Base &base) {
        Base::operator=(base);
        return *this;
    }

    Path &operator=(Base &&base) noexcept {
        Base::operator=(std::move(base));
        return *this;
    }

    Path &operator=(const SystemString &str) {
        Base::operator=(str.native());
        return *this;
    }

    Path &operator=(SystemString &&str) {
        Base::operator=(std::move(str).native());
        return *this;
    }

    Path &operator=(const char *str) {
        return operator=(SystemString(str));
    }

    Path &operator=(const std::string &str) {
        return operator=(SystemString(str));
    }

    Path &operator=(std::string_view str) {
        return operator=(SystemString(str));
    }

    Path &operator=(const Path &other) = default;
    Path &operator=(Path &&other) noexcept = default;

    Base &base() noexcept {
        return static_cast<Base &>(*this);
    }

    const Base &base() const noexcept {
        return static_cast<const Base &>(*this);
    }

    SystemString to_system_string() const {
        return SystemString(Base::native());
    }

    std::string to_string() const {
        return to_system_string().to_string();
    }

    bool empty() const {
        return Base::empty();
    }

    bool has_root_path() const {
        return Base::has_root_path();
    }

    bool has_root_name() const {
        return Base::has_root_name();
    }

    bool has_root_directory() const {
        return Base::has_root_directory();
    }

    bool has_relative_path() const {
        return Base::has_relative_path();
    }

    bool has_parent_path() const {
        return Base::has_parent_path();
    }

    bool has_filename() const {
        return Base::has_filename();
    }

    bool has_stem() const {
        return Base::has_stem();
    }

    bool has_extension() const {
        return Base::has_extension();
    }

    bool is_absolute() const {
        return Base::is_absolute();
    }

    bool is_relative() const {
        return Base::is_relative();
    }

    Path root_path() const {
        return Base::root_path();
    }

    Path root_name() const {
        return Base::root_name();
    }

    Path root_directory() const {
        return Base::root_directory();
    }

    Path relative_path() const {
        return Base::relative_path();
    }

    Path parent_path() const {
        return Base::parent_path();
    }

    Path filename() const {
        return Base::filename();
    }

    Path stem() const {
        return Base::stem();
    }

    Path extension() const {
        return Base::extension();
    }

    Path lexically_normal() const {
        return Base::lexically_normal();
    }

    Path lexically_relative(const Path &base) const {
        return Base::lexically_relative(base);
    }

    Path lexically_proximate(const Path &base) const {
        return Base::lexically_proximate(base);
    }

    Path &make_preferred() {
        Base::make_preferred();
        return *this;
    }

    Path &remove_filename() {
        Base::remove_filename();
        return *this;
    }

    Path &replace_filename(const Path &replacement) {
        Base::replace_filename(replacement);
        return *this;
    }

    Path &replace_extension(const Path &replacement) {
        Base::replace_extension(replacement);
        return *this;
    }

    Path &operator/=(const Path &other) {
        Base::operator/=(other);
        return *this;
    }

    Path &operator/=(const Base &other) {
        Base::operator/=(other);
        return *this;
    }

    Path &operator/=(const SystemString &other) {
        Base::operator/=(other.native());
        return *this;
    }

    Path &operator/=(const char *str) {
        return operator/=(SystemString(str));
    }

    Path &operator/=(const std::string &str) {
        return operator/=(SystemString(str));
    }

    Path &operator/=(std::string_view str) {
        return operator/=(SystemString(str));
    }

    Path &operator+=(const Path &other) {
        Base::operator+=(other);
        return *this;
    }

    Path &operator+=(const Base &base) {
        Base::operator+=(base);
        return *this;
    }

    Path &operator+=(const SystemString &str) {
        Base::operator+=(str.native());
        return *this;
    }

    Path &operator+=(const char *str) {
        return operator+=(SystemString(str));
    }

    Path &operator+=(const std::string &str) {
        return operator+=(SystemString(str));
    }

    Path &operator+=(std::string_view str) {
        return operator+=(SystemString(str));
    }

    void clear() {
        Base::clear();
    }

    void swap(Path &other) noexcept {
        Base::swap(other);
    }
};

inline void swap(Path &lhs, Path &rhs) noexcept {
    lhs.swap(rhs);
}

inline bool operator==(const Path &lhs, const Path &rhs) noexcept {
    return lhs.base() == rhs.base();
}

inline std::strong_ordering operator<=>(const Path &lhs, const Path &rhs) noexcept {
    return lhs.base() <=> rhs.base();
}

inline Path operator/(Path lhs, const Path &rhs) {
    return lhs /= rhs;
}

} // namespace mini_llvm

template <>
struct std::hash<mini_llvm::Path> {
    size_t operator()(const mini_llvm::Path &path) const noexcept {
        return std::hash<std::filesystem::path>()(path.base());
    }
};

template <>
struct std::formatter<mini_llvm::Path> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::Path &path, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", path.to_string());
    }
};
