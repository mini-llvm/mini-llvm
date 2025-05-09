#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <getopt.h>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/common/SourceManger.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Verify.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
#include "mini-llvm/utils/Expected.h"
#include "mini-llvm/utils/FileSystem.h"
#include "mini-llvm/utils/ProcessorDetection.h"

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

struct Options {
    Target target;
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    bool dumpIR;
    bool dumpMIR;
};

} // namespace

int main(int argc, char *argv[]) {
    Options options{};

    const char *shortOpts = "o:";
    struct option longOpts[] = {
        {"help", no_argument, nullptr, CHAR_MAX + 1},
        {"target", required_argument, nullptr, CHAR_MAX + 2},
        {"dump-ir", no_argument, nullptr, CHAR_MAX + 3},
        {"dump-mir", no_argument, nullptr, CHAR_MAX + 4},
        {nullptr, 0, nullptr, 0},
    };

    int opt;
    while ((opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1) {
        switch (opt) {
            case CHAR_MAX + 1: {
                std::println(stdout, "Usage: {} [--target=<target>] [-o <output-file>] <input-file>", argv[0]);
                return EXIT_SUCCESS;
            }

            case CHAR_MAX + 2: {
                Target target = toTarget(optarg);
                if (target == Target::kNone) {
                    std::println(stderr, "{}: error: unsupported target '%s'", argv[0], optarg);
                    return EXIT_FAILURE;
                }
                options.target = target;
                break;
            }

            case CHAR_MAX + 3: {
                options.dumpIR = true;
                break;
            }

            case CHAR_MAX + 4: {
                options.dumpMIR = true;
                break;
            }

            case 'o': {
                options.outputFile = optarg;
                break;
            }

            default: {
                return EXIT_FAILURE;
            }
        }
    }

    if (optind == argc) {
        std::println(stderr, "{}: error: no input file", argv[0]);
        return EXIT_FAILURE;
    }
    if (optind < argc - 1) {
        std::println(stderr, "{}: error: multiple input files", argv[0]);
        return EXIT_FAILURE;
    }
    options.inputFile = argv[optind];

    if (options.outputFile.empty()) {
        options.outputFile = options.inputFile;
        options.outputFile.replace_extension(".s");
    }

    if (options.target == Target::kNone) {
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
        Target target = toTarget(targetName);
        if (target == Target::kNone) {
            std::println(stderr, "{}: error: unsupported target '{}'", argv[0], targetName);
            return EXIT_FAILURE;
        }
        options.target = target;
    }

    Expected<std::string, int> source = readAll(options.inputFile);
    if (!source) {
        std::println(stderr, "{}: error: {}: {}", argv[0], options.inputFile.c_str(), strerror(source.error()));
        return EXIT_FAILURE;
    }
    if (!source->empty() && source->back() != '\n') {
        source->push_back('\n');
    }
    SourceManager sourceManager;
    sourceManager.setSource(std::move(*source));

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> IM = ir::parseModule(sourceManager.source().c_str(), diags);
    for (const Diagnostic &diag : diags) {
        auto [line, column] = sourceManager.lineColumn(diag.location);
        std::println(stderr, "{}:{}:{}: {}: {}", options.inputFile.c_str(), line + 1, column + 1, name(diag.level), diag.message);
        if (line < sourceManager.lineCount()) {
            std::print(stderr, "{}", sourceManager.line(line));
            std::println(stderr, "{}^", std::string(column, ' '));
        }
    }
    if (!IM) {
        return EXIT_FAILURE;
    }

    if (!ir::verifyModule(*IM)) {
        std::println(stderr, "{}: error: invalid module", argv[0]);
        return EXIT_FAILURE;
    }

    ir::PassManager passManager;
    passManager.run(*IM);

    if (options.dumpIR) {
        std::println(stderr, "{}", IM->format());
    }

    mir::Module MM;
    mc::Program program;

    if (options.target == Target::kRISCV64) {
        RISCVBackendDriver backendDriver;
        backendDriver.run(*IM, MM, program);
    }

    if (options.dumpMIR) {
        std::println(stderr, "{}", MM.format());
    }

    std::string output = program.format() + '\n';

    if (Expected<void, int> result = writeAll(options.outputFile, output); !result) {
        std::println(stderr, "{}: error: {}: {}", argv[0], options.outputFile.c_str(), strerror(result.error()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
