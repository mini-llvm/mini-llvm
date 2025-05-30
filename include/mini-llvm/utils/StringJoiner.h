#pragma once

#include <format>
#include <string>
#include <string_view>

namespace mini_llvm {

class StringJoiner {
public:
    explicit StringJoiner(std::string_view delimiter)
        : delimiter_(delimiter), first_(true) {}

    StringJoiner(std::string_view delimiter, std::string_view prefix, std::string_view suffix)
        : delimiter_(delimiter), prefix_(prefix), suffix_(suffix), first_(true) {}

    StringJoiner &add(std::string_view element) {
        if (first_) {
            first_ = false;
        } else {
            out_ += delimiter_;
        }
        out_ += element;
        return *this;
    }

    template <typename... Args>
    StringJoiner &add(std::format_string<Args...> fmt, Args &&...args) {
        return add(std::vformat(fmt.get(), std::make_format_args(args...)));
    }

    std::string toString() const {
        return std::string(prefix_) + out_ + std::string(suffix_);
    }

private:
    std::string_view delimiter_, prefix_, suffix_;
    std::string out_;
    bool first_;
};

} // namespace mini_llvm

template <>
struct std::formatter<mini_llvm::StringJoiner> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::StringJoiner &joiner, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", joiner.toString());
    }
};
