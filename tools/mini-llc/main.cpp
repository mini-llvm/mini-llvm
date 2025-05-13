#include <cstdio>
#include <cstdlib>
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
    kNone,
    kRISCV64,
};

Target toTarget(std::string_view targetName) {
    if (targetName == "riscv64") {
        return Target::kRISCV64;
    }
    return Target::kNone;
}

} // namespace

int mainImpl(std::vector<std::string> args) {
    CommandLineParser parser;

    parser.addOption("--help");
    parser.addOption("--target:");
    parser.addOption("-o:");
    parser.addOption("--dump-ir");
    parser.addOption("--dump-mir");

    if (Expected<void, CommandLineParser::Error> result = parser.parse(args); !result) {
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
        return EXIT_FAILURE;
    }

    Target target = Target::kNone;
    std::string inputFile;
    std::string outputFile;
    bool dumpIR = false;
    bool dumpMIR = false;

    for (const CommandLineParser::Argument &arg : parser) {
        if (arg.isOption()) {
            if (arg.name() == "--help") {
                std::println(stdout, "Usage: {} [--target=<target>] [-o <output-file>] <input-file>", args[0]);
                return EXIT_SUCCESS;
            }
            if (arg.name() == "--target") {
                target = toTarget(arg.value());
                if (target == Target::kNone) {
                    std::println(stderr, "{}: error: unsupported target '%s'", args[0], arg.value());
                    return EXIT_FAILURE;
                }
                continue;
            }
            if (arg.name() == "-o") {
                outputFile = arg.value();
                continue;
            }
            if (arg.name() == "--dump-ir") {
                dumpIR = true;
                continue;
            }
            if (arg.name() == "--dump-mir") {
                dumpMIR = true;
                continue;
            }
        }
        if (!inputFile.empty()) {
            std::println(stderr, "{}: error: multiple input files", args[0]);
            return EXIT_FAILURE;
        }
        inputFile = arg.arg();
    }

    if (inputFile.empty()) {
        std::println(stderr, "{}: error: no input file", args[0]);
        return EXIT_FAILURE;
    }

    if (outputFile.empty()) {
        outputFile = inputFile;
        size_t i = outputFile.rfind('.');
        if (i == std::string::npos) {
            outputFile += ".s";
        } else {
            outputFile = outputFile.substr(0, i) + ".s";
        }
    }

    if (target == Target::kNone) {
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
        if (target == Target::kNone) {
            std::println(stderr, "{}: error: unsupported target '{}'", args[0], targetName);
            return EXIT_FAILURE;
        }
    }

    Expected<std::string, int> source = readAll(inputFile);
    if (!source) {
        std::println(stderr, "{}: error: {}: {}", args[0], inputFile, strerror(source.error()));
        return EXIT_FAILURE;
    }
    if (!source->empty() && source->back() != '\n') {
        source->push_back('\n');
    }
    SourceManager sourceManager;
    sourceManager.setSource(std::move(*source));

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> IM = ir::parseModule(sourceManager.source(), diags);
    for (const Diagnostic &diag : diags) {
        auto [line, column] = sourceManager.lineColumn(diag.location);
        std::println(stderr, "{}:{}:{}: {}: {}", inputFile, line + 1, column + 1, name(diag.level), diag.message);
        if (line < sourceManager.lineCount()) {
            std::print(stderr, "{}", sourceManager.line(line));
            std::println(stderr, "{}^", std::string(column, ' '));
        }
    }
    if (!IM) {
        return EXIT_FAILURE;
    }

    if (!ir::verifyModule(*IM)) {
        std::println(stderr, "{}: error: invalid module", args[0]);
        return EXIT_FAILURE;
    }

    ir::PassManager passManager;
    passManager.run(*IM);

    if (dumpIR) {
        std::println(stderr, "{}", IM->format());
    }

    mir::Module MM;
    mc::Program program;

    if (target == Target::kRISCV64) {
        RISCVBackendDriver backendDriver;
        backendDriver.run(*IM, MM, program);
    }

    if (dumpMIR) {
        std::println(stderr, "{}", MM.format());
    }

    std::string output = program.format() + '\n';

    if (Expected<void, int> result = writeAll(outputFile, output.data(), output.size()); !result) {
        std::println(stderr, "{}: error: {}: {}", args[0], outputFile, strerror(result.error()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
