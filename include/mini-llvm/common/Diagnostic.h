#pragma once

#include <filesystem>
#include <format>
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
    size_t line;
    size_t column;

    std::string format() const {
        const char *severityName;
        switch (severity) {
            case Severity::kNote: severityName = "note"; break;
            case Severity::kWarning: severityName = "warning"; break;
            case Severity::kError: severityName = "error"; break;
        }
        return std::format("{}:{}:{}: {}: {}", file.c_str(), line, column, severityName, message);
    }

    static Diagnostic note(std::string message, std::filesystem::path file, size_t line, size_t column) {
        return {Severity::kNote, std::move(message), std::move(file), line, column};
    }

    static Diagnostic warning(std::string message, std::filesystem::path file, size_t line, size_t column) {
        return {Severity::kWarning, std::move(message), std::move(file), line, column};
    }

    static Diagnostic error(std::string message, std::filesystem::path file, size_t line, size_t column) {
        return {Severity::kError, std::move(message), std::move(file), line, column};
    }
};

} // namespace mini_llvm
