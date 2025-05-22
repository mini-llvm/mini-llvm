#include <cstdio>
#include <cstring>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/common/SourceManger.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Verify.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
#include "mini-llvm/utils/Colors.h"
#include "mini-llvm/utils/CommandLineParser.h"
#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileSystem.h"
#include "mini-llvm/utils/ProcessorDetection.h"

#ifdef _WIN32
    #include "mini-llvm/utils/Windows.h"
#endif

using namespace mini_llvm;

using enum Color;

namespace {

enum class Target {
    kRISCV64,
};

std::optional<Target> toTarget(std::string_view targetName) {
    if (targetName == "riscv64") {
        return Target::kRISCV64;
    }
    return std::nullopt;
}

} // namespace

int mainImpl(std::vector<std::string> args) {
    bool showColor = supportsColor(stderr);

    CommandLineParser parser;

    parser.addOption("--help");
    parser.addOption("--target:");
    parser.addOption("-o:");
    parser.addOption("--dump-ir::");
    parser.addOption("--dump-mir::");

    if (Expected<void, CommandLineParser::Error> result = parser.parse(args); !result) {
        using enum CommandLineParser::ErrorKind;
        switch (result.error().kind()) {
        case kMissingValue:
            print(stderr, showColor, "{}: {}error: {}missing value to '{}'\n", args[0], kBold + kRed, kReset, result.error().optionName());
            break;

        case kUnexpectedValue:
            print(stderr, showColor, "{}: {}error: {}unexpected value to '{}'\n", args[0], kBold + kRed, kReset, result.error().optionName());
            break;

        case kUnrecognizedOption:
            print(stderr, showColor, "{}: {}error: {}unrecognized option '{}'\n", args[0], kBold + kRed, kReset, result.error().optionName());
            break;
        }
        return 1;
    }

    std::optional<Target> target;
    std::optional<std::string> inputFile;
    std::optional<std::string> outputFile;
    std::optional<std::string> irDumpFile;
    std::optional<std::string> mirDumpFile;

    for (const CommandLineParser::Argument &arg : parser) {
        if (arg.isOption()) {
            if (arg.name() == "--help") {
                std::print(stdout, "Usage: {} [--target=<target>] [-o <output-file>] <input-file>\n", args[0]);
                return 0;
            }
            if (arg.name() == "--target") {
                target = toTarget(arg.value());
                if (!target) {
                    print(stderr, showColor, "{}: {}error: {}unsupported target '{}'\n", args[0], kBold + kRed, kReset, arg.value());
                    return 1;
                }
                continue;
            }
            if (arg.name() == "-o") {
                outputFile = arg.value();
                continue;
            }
            if (arg.name() == "--dump-ir") {
                irDumpFile = arg.valueOr("-");
                continue;
            }
            if (arg.name() == "--dump-mir") {
                mirDumpFile = arg.valueOr("-");
                continue;
            }
        }
        inputFile = arg.arg();
    }

    if (!inputFile) {
        inputFile = "-";
    }

    if (!outputFile) {
        outputFile = *inputFile;
        if (*outputFile != "-") {
            size_t i = outputFile->rfind('.');
            if (i != std::string::npos) {
                outputFile->erase(i);
            }
            outputFile->append(".s");
        }
    }

    if (!target) {
        const char *targetName;
#if defined(MINI_LLVM_X86_64)
        targetName = "x86_64";
#elif defined(MINI_LLVM_I386)
        targetName = "i386";
#elif defined(MINI_LLVM_AARCH64)
        targetName = "aarch64";
#elif defined(MINI_LLVM_ARM)
        targetName = "arm";
#elif defined(MINI_LLVM_PPC64)
        targetName = "ppc64";
#elif defined(MINI_LLVM_PPC)
        targetName = "ppc";
#elif defined(MINI_LLVM_RISCV64)
        targetName = "riscv64";
#elif defined(MINI_LLVM_RISCV32)
        targetName = "riscv32";
#else
        targetName = "unknown";
#endif
        target = toTarget(targetName);
        if (!target) {
            print(stderr, showColor, "{}: {}error: {}unsupported target '{}'\n", args[0], kBold + kRed, kReset, targetName);
            return 1;
        }
    }

    Expected<std::string, int> source;
    if (*inputFile == "-") {
        source = readAll(stdin);
    } else {
        source = readAll(*inputFile);
    }
    if (!source) {
        print(stderr, showColor, "{}: {}error: {}{}: {}\n", args[0], kBold + kRed, kReset, *inputFile, strerror(source.error()));
        return 1;
    }
    if (!source->empty() && source->back() != '\n') {
        source->push_back('\n');
    }
    SourceManager sourceManager;
    sourceManager.setSource(std::move(*source));

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> IM = ir::parseModule(sourceManager.source(), diags);
    if (*inputFile == "-") {
        *inputFile = "<stdin>";
    }
    for (const Diagnostic &diag : diags) {
        auto [line, column] = sourceManager.lineColumn(diag.location);
        print(stderr, showColor, "{}:{}:{}: ", *inputFile, line + 1, column + 1);
        switch (diag.level) {
        case Diagnostic::Level::kNote:
            print(stderr, showColor, "{}note: {}", kBold + kCyan, kReset);
            break;
        case Diagnostic::Level::kWarning:
            print(stderr, showColor, "{}warning: {}", kBold + kMagenta, kReset);
            break;
        case Diagnostic::Level::kError:
            print(stderr, showColor, "{}error: {}", kBold + kRed, kReset);
            break;
        }
        print(stderr, showColor, "{}\n", diag.message);
        if (line < sourceManager.lineCount()) {
            print(stderr, showColor, "{}", sourceManager.line(line));
            print(stderr, showColor, "{}{}^{}\n", std::string(column, ' '), kBold + kGreen, kReset);
        }
    }
    if (!IM) {
        return 1;
    }

    if (!ir::verifyModule(*IM)) {
        print(stderr, showColor, "{}: {}error: {}invalid module\n", args[0], kBold + kRed, kReset);
        return 1;
    }

    int pointerSize;

    switch (*target) {
    case Target::kRISCV64:
        pointerSize = 8;
        break;
    }

    ir::PassManager passManager(pointerSize);
    passManager.run(*IM);

    if (irDumpFile) {
        std::string output = IM->format() + '\n';
        Expected<void, int> result;
        if (*irDumpFile == "-") {
            result = writeAll(stdout, output.data(), output.size());
        } else {
            result = writeAll(*irDumpFile, output.data(), output.size());
        }
        if (!result) {
            print(stderr, showColor, "{}: {}error: {}{}: {}\n", args[0], kBold + kRed, kReset, *irDumpFile, strerror(result.error()));
            return 1;
        }
    }

    mir::Module MM;
    mc::Program program;

    switch (*target) {
        case Target::kRISCV64: {
            RISCVBackendDriver backendDriver;
            backendDriver.run(*IM, MM, program);
            break;
        }
    }

    if (mirDumpFile) {
        std::string output = MM.format() + '\n';
        Expected<void, int> result;
        if (*mirDumpFile == "-") {
            result = writeAll(stdout, output.data(), output.size());
        } else {
            result = writeAll(*mirDumpFile, output.data(), output.size());
        }
        if (!result) {
            print(stderr, showColor, "{}: {}error: {}{}: {}\n", args[0], kBold + kRed, kReset, *mirDumpFile, strerror(result.error()));
            return 1;
        }
    }

    std::string output = program.format() + '\n';
    Expected<void, int> result;
    if (*outputFile == "-") {
        result = writeAll(stdout, output.data(), output.size());
    } else {
        result = writeAll(*outputFile, output.data(), output.size());
    }
    if (!result) {
        print(stderr, showColor, "{}: {}error: {}{}: {}\n", args[0], kBold + kRed, kReset, *outputFile, strerror(result.error()));
        return 1;
    }

    return 0;
}

#ifdef _WIN32

int wmain(int argc, wchar_t *wargv[]) {
    return mainImpl(windows::narrowArgs(argc, wargv));
}

#else

int main(int argc, char *argv[]) {
    return mainImpl(std::vector<std::string>(argv, argv + argc));
}

#endif
