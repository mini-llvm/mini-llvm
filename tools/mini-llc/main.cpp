#include <cstddef>
#include <cstdio>
#include <cstring>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/codegen/register_allocators/GraphColoringAllocator.h"
#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/common/SourceManager.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Verifier.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
#include "mini-llvm/utils/CommandLineParser.h"
#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileSystem.h"
#include "mini-llvm/utils/ProcessorDetection.h"

#ifdef _WIN32
    #include "mini-llvm/utils/Windows.h"
#endif

using namespace mini_llvm;

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

Expected<std::string, int> input(const std::string &inputFile) {
    if (inputFile == "-") {
        return readAll(stdin);
    } else {
        return readAll(inputFile);
    }
}

Expected<void, int> output(const std::string &outputFile, const std::string &content) {
    if (outputFile == "-") {
        return writeAll(stdout, content.data(), content.size());
    } else {
        return writeAll(outputFile, content.data(), content.size());
    }
}

int mainImpl(std::vector<std::string> args) {
    CommandLineParser parser;

    parser.addOption("--help");
    parser.addOption("--target:");
    parser.addOption("-o:");
    parser.addOption("--dump-ir::");
    parser.addOption("--dump-mir::");

    if (Expected<void, CommandLineParser::Error> result = parser(args); !result) {
        using enum CommandLineParser::ErrorKind;
        switch (result.error().kind()) {
        case kMissingValue:
            std::println(stderr, "{}: error: missing value to '{}'", args[0], result.error().optionName());
            break;

        case kUnexpectedValue:
            std::println(stderr, "{}: error: unexpected value to '{}'", args[0], result.error().optionName());
            break;

        case kUnrecognizedOption:
            std::println(stderr, "{}: error: unrecognized option '{}'", args[0], result.error().optionName());
            break;
        }
        return 1;
    }

    std::optional<Target> target;
    std::optional<std::string> inputFile;
    std::optional<std::string> outputFile;
    std::optional<std::string> irDumpFile;
    std::optional<std::string> mirDumpFile;

    for (const auto &arg : parser) {
        if (const auto *option = arg.option()) {
            if (option->name() == "--help") {
                std::println(stdout, "Usage: {} [--target=<target>] [-o <output-file>] <input-file>", args[0]);
                return 0;
            }
            if (option->name() == "--target") {
                target = toTarget(*option->value());
                if (!target) {
                    std::println(stderr, "{}: error: unsupported target '{}'", args[0], *option->value());
                    return 1;
                }
                continue;
            }
            if (option->name() == "-o") {
                outputFile = *option->value();
                continue;
            }
            if (option->name() == "--dump-ir") {
                irDumpFile = option->value().value_or("-");
                continue;
            }
            if (option->name() == "--dump-mir") {
                mirDumpFile = option->value().value_or("-");
                continue;
            }
        }
        inputFile = arg.positional()->arg();
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
            std::println(stderr, "{}: error: unsupported target '{}'", args[0], targetName);
            return 1;
        }
    }

    Expected<std::string, int> source = input(*inputFile);
    if (!source) {
        std::println(stderr, "{}: error: {}: {}", args[0], *inputFile, strerror(source.error()));
        return 1;
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
        std::println(stderr, "{}:{}:{}: {}: {}", *inputFile, line + 1, column + 1, name(diag.level), diag.message);
        if (line < sourceManager.lineCount()) {
            std::print(stderr, "{}", sourceManager.line(line));
            std::println(stderr, "{}^", std::string(column, ' '));
        }
    }
    if (!IM) {
        return 1;
    }

    if (!ir::verifyModule(*IM)) {
        std::println(stderr, "{}: error: ill-formed module", args[0]);
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
        if (Expected<void, int> result = output(*irDumpFile, IM->format() + '\n'); !result) {
            std::println(stderr, "{}: error: {}: {}", args[0], *irDumpFile, strerror(result.error()));
            return 1;
        }
    }

    mir::Module MM;
    mc::Module MCM;

    GraphColoringAllocator allocator;

    switch (*target) {
        case Target::kRISCV64: {
            RISCVBackendDriver backendDriver(&allocator);
            backendDriver.run(*IM, MM, MCM);
            break;
        }
    }

    if (mirDumpFile) {
        if (Expected<void, int> result = output(*mirDumpFile, MM.format() + '\n'); !result) {
            std::println(stderr, "{}: error: {}: {}", args[0], *mirDumpFile, strerror(result.error()));
            return 1;
        }
    }

    if (Expected<void, int> result = output(*outputFile, MCM.format() + '\n'); !result) {
        std::println(stderr, "{}: error: {}: {}", args[0], *outputFile, strerror(result.error()));
        return 1;
    }

    return 0;
}

} // namespace

#ifdef _WIN32

int wmain(int argc, wchar_t *wargv[]) {
    return mainImpl(windows::narrowArgs(argc, wargv));
}

#else

int main(int argc, char *argv[]) {
    return mainImpl(std::vector<std::string>(argv, argv + argc));
}

#endif
