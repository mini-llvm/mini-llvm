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
};

} // namespace mini_llvm
