// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/codegen/register_allocators/GraphColoringAllocator.h"
#include "mini-llvm/codegen/register_allocators/LinearScanAllocator.h"
#include "mini-llvm/codegen/register_allocators/NaiveAllocator.h"
#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/common/SourceManager.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
#include "mini-llvm/utils/CommandLineParser.h"
#include "mini-llvm/utils/ErrorCode.h"
#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/IO.h"
#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/ProcessorDetection.h"
#include "mini-llvm/utils/Strings.h"
#include "mini-llvm/utils/SystemError.h"

#ifdef _WIN32
    #include "mini-llvm/utils/Windows.h"
#endif

using namespace mini_llvm;

namespace {

enum class TargetOption {
    kRISCV64,
};

std::optional<TargetOption> toTargetOption(std::string_view targetName) {
    if (targetName == "riscv64") {
        return TargetOption::kRISCV64;
    }
    return std::nullopt;
}

enum class RegisterAllocatorOption {
    kGraphColoring,
    kLinearScan,
    kNaive,
};

std::optional<RegisterAllocatorOption> toRegisterAllocatorOption(std::string_view registerAllocatorName) {
    if (registerAllocatorName == "graph-coloring") {
        return RegisterAllocatorOption::kGraphColoring;
    }
    if (registerAllocatorName == "linear-scan") {
        return RegisterAllocatorOption::kLinearScan;
    }
    if (registerAllocatorName == "naive") {
        return RegisterAllocatorOption::kNaive;
    }
    return std::nullopt;
}

struct Options {
    std::optional<TargetOption> target;
    std::optional<RegisterAllocatorOption> registerAllocator;
    std::optional<Path> inputFile;
    std::optional<Path> outputFile;
    std::optional<Path> irDumpFile;
    std::optional<Path> mirDumpFile;
};

int mainImpl(std::vector<std::string> args) {
    CommandLineParser parser;

    parser.addOption("--help");
    parser.addOption("--target:");
    parser.addOption("--register-allocator:");
    parser.addOption("-o:");
    parser.addOption("--dump-ir::");
    parser.addOption("--dump-mir::");

    auto parseResult = parser(args);

    if (!parseResult) {
        using enum CommandLineParser::ErrorKind;
        switch (parseResult.error().kind()) {
        case kMissingValue:
            std::println(stderr, "{}: error: missing value to '{}'", args[0], parseResult.error().optionName());
            break;

        case kUnexpectedValue:
            std::println(stderr, "{}: error: unexpected value to '{}'", args[0], parseResult.error().optionName());
            break;

        case kUnrecognizedOption:
            std::println(stderr, "{}: error: unrecognized option '{}'", args[0], parseResult.error().optionName());
            break;
        }
        return EXIT_FAILURE;
    }

    Options options;

    for (const auto &arg : *parseResult) {
        if (const auto *option = arg.option()) {
            if (option->name() == "--help") {
                std::println(stdout, "Usage: {} [--target=<target>] [-o <output-file>] <input-file>", args[0]);
                return EXIT_SUCCESS;
            }
            if (option->name() == "--target") {
                options.target = toTargetOption(*option->value());
                if (!options.target) {
                    std::println(stderr, "{}: error: unsupported target '{}'", args[0], *option->value());
                    return EXIT_FAILURE;
                }
                continue;
            }
            if (option->name() == "--register-allocator") {
                options.registerAllocator = toRegisterAllocatorOption(*option->value());
                if (!options.registerAllocator) {
                    std::println(stderr, "{}: error: invalid register allocator '{}'", args[0], *option->value());
                    return EXIT_FAILURE;
                }
                continue;
            }
            if (option->name() == "-o") {
                options.outputFile = *option->value();
                continue;
            }
            if (option->name() == "--dump-ir") {
                options.irDumpFile = option->value().value_or("-");
                continue;
            }
            if (option->name() == "--dump-mir") {
                options.mirDumpFile = option->value().value_or("-");
                continue;
            }
        }
        if (const auto *positional = arg.positional()) {
            options.inputFile = positional->arg();
        }
    }

    if (!options.inputFile) {
        options.inputFile = "-";
    }

    if (!options.outputFile) {
        options.outputFile = *options.inputFile;
        if (*options.outputFile != "-") {
            options.outputFile->replace_extension(".s");
        }
    }

    if (!options.target) {
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
        options.target = toTargetOption(targetName);
        if (!options.target) {
            std::println(stderr, "{}: error: unsupported target '{}'", args[0], targetName);
            return EXIT_FAILURE;
        }
    }

    if (!options.registerAllocator) {
        options.registerAllocator = RegisterAllocatorOption::kGraphColoring;
    }

    Expected<std::string, SystemError> source = readAll(*options.inputFile, stdin);
    if (!source) {
        std::println(stderr, "{}: error: {}: {}", args[0], *options.inputFile, message(source.error().code()));
        return EXIT_FAILURE;
    }
    normalizeLineEndings(*source);
    SourceManager sourceManager;
    sourceManager.setSource(*std::move(source));

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> IM = ir::parseModule(sourceManager.source(), diags);
    if (*options.inputFile == "-") {
        *options.inputFile = "<stdin>";
    }
    for (const Diagnostic &diag : diags) {
        auto [line, column] = sourceManager.lineColumn(diag.location);
        std::println(stderr, "{}:{}:{}: {}: {}", *options.inputFile, line + 1, column + 1, name(diag.level), diag.message);
        if (line < sourceManager.lineCount()) {
            std::print(stderr, "{}", sourceManager.line(line));
            std::println(stderr, "{}^", std::string(column, ' '));
        }
    }
    if (!IM) {
        return EXIT_FAILURE;
    }

    if (!IM->isWellFormed()) {
        std::println(stderr, "{}: error: ill-formed module", args[0]);
        return EXIT_FAILURE;
    }

    int pointerSize;

    switch (*options.target) {
    case TargetOption::kRISCV64:
        pointerSize = 8;
        break;
    }

    ir::PassManager passManager(pointerSize);
    passManager.run(*IM);

    if (options.irDumpFile) {
        if (Expected<void, SystemError> result = writeAll(*options.irDumpFile, stdout, std::format("{}\n", *IM)); !result) {
            std::println(stderr, "{}: error: {}: {}", args[0], *options.irDumpFile, message(result.error().code()));
            return EXIT_FAILURE;
        }
    }

    mir::Module MM;
    mc::Module MCM;

    std::unique_ptr<RegisterAllocator> allocator;
    switch (*options.registerAllocator) {
        case RegisterAllocatorOption::kGraphColoring:
            allocator = std::make_unique<GraphColoringAllocator>();
            break;
        case RegisterAllocatorOption::kLinearScan:
            allocator = std::make_unique<LinearScanAllocator>();
            break;
        case RegisterAllocatorOption::kNaive:
            allocator = std::make_unique<NaiveAllocator>();
            break;
    }

    switch (*options.target) {
        case TargetOption::kRISCV64: {
            RISCVBackendDriver backendDriver(&*allocator);
            backendDriver.run(*IM, MM, MCM);
            break;
        }
    }

    if (options.mirDumpFile) {
        if (Expected<void, SystemError> result = writeAll(*options.mirDumpFile, stdout, std::format("{}\n", MM)); !result) {
            std::println(stderr, "{}: error: {}: {}", args[0], *options.mirDumpFile, message(result.error().code()));
            return EXIT_FAILURE;
        }
    }

    if (Expected<void, SystemError> result = writeAll(*options.outputFile, stdout, std::format("{}\n", MCM)); !result) {
        std::println(stderr, "{}: error: {}: {}", args[0], *options.outputFile, message(result.error().code()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

} // namespace

#ifdef _WIN32

int wmain(int argc, wchar_t *wargv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.push_back(windows::narrow(wargv[i]));
    }
    return mainImpl(std::move(args));
}

#else

int main(int argc, char *argv[]) {
    return mainImpl(std::vector<std::string>(argv, argv + argc));
}

#endif
