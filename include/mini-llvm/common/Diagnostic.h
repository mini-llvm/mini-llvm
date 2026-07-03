// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdlib>
#include <string>
#include <utility>

#include "mini-llvm/utils/Color.h"

namespace mini_llvm {

struct Diagnostic {
    enum class Level {
        kNote,
        kWarning,
        kError,
    };

    Level level;
    std::string message;
    size_t location;

    static Diagnostic note(std::string message, size_t location) {
        return {Level::kNote, std::move(message), location};
    }

    static Diagnostic warning(std::string message, size_t location) {
        return {Level::kWarning, std::move(message), location};
    }

    static Diagnostic error(std::string message, size_t location) {
        return {Level::kError, std::move(message), location};
    }
};

constexpr const char *name(Diagnostic::Level level) {
    using enum Diagnostic::Level;
    switch (level) {
        case kNote: return "note";
        case kWarning: return "warning";
        case kError: return "error";
        default: abort();
    }
}

inline Color color(Diagnostic::Level level) {
    using enum Diagnostic::Level;
    using namespace colors;
    switch (level) {
        case kNote: return cyan;
        case kWarning: return magenta;
        case kError: return red;
        default: abort();
    }
}

} // namespace mini_llvm
