#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

namespace mini_llvm {

struct Diagnostic {
    enum class Severity {
        kNote,
        kWarning,
        kError,
    };

    Severity severity;
    std::string message;
    std::filesystem::path file;
    size_t offset;

    static Diagnostic note(std::string message, std::filesystem::path file, size_t offset) {
        return {Severity::kNote, std::move(message), std::move(file), offset};
    }

    static Diagnostic warning(std::string message, std::filesystem::path file, size_t offset) {
        return {Severity::kWarning, std::move(message), std::move(file), offset};
    }

    static Diagnostic error(std::string message, std::filesystem::path file, size_t offset) {
        return {Severity::kError, std::move(message), std::move(file), offset};
    }
};

inline constexpr const char *name(Diagnostic::Severity severity) {
    using enum Diagnostic::Severity;
    switch (severity) {
        case kNote: return "note";
        case kWarning: return "warning";
        case kError: return "error";
        default: abort();
    }
}

} // namespace mini_llvm
