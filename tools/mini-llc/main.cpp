#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <getopt.h>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Verify.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
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
                fprintf(stdout, "Usage: %s [--target=<target>] [-o <output-file>] <input-file>\n", argv[0]);
                return EXIT_SUCCESS;
            }

            case CHAR_MAX + 2: {
                Target target = toTarget(optarg);
                if (target == Target::kNone) {
                    fprintf(stderr, "%s: error: unsupported target '%s'\n", argv[0], optarg);
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
                if (*optarg == '\0') {
                    fprintf(stderr, "%s: error: output file cannot be empty\n", argv[0]);
                    return EXIT_FAILURE;
                }
                options.outputFile = optarg;
                break;
            }

            default: {
                return EXIT_FAILURE;
            }
        }
    }

    if (optind == argc) {
        fprintf(stderr, "%s: error: no input file\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (optind < argc - 1) {
        fprintf(stderr, "%s: error: multiple input files\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (*argv[optind] == '\0') {
        fprintf(stderr, "%s: error: input file cannot be empty\n", argv[0]);
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
            fprintf(stderr, "%s: error: unsupported target '%s'\n", argv[0], targetName);
            return EXIT_FAILURE;
        }
        options.target = target;
    }

    Expected<std::string, int> input = readAll(options.inputFile);
    if (!input) {
        fprintf(stderr, "%s: error: %s\n", argv[0], strerror(input.error()));
        return EXIT_FAILURE;
    }

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> M = ir::parseModule(input.value().c_str(), diags);
    if (!M) {
        for (Diagnostic &diag : diags) {
            diag.file = options.inputFile;
        }
        for (const Diagnostic &diag : diags) {
            fprintf(stderr, "%s\n", diag.format().c_str());
        }
        return EXIT_FAILURE;
    }

    if (!ir::verifyModule(*M)) {
        fprintf(stderr, "%s: error: invalid module\n", argv[0]);
        return EXIT_FAILURE;
    }

    ir::PassManager passManager;
    passManager.run(*M);

    if (options.dumpIR) {
        fprintf(stderr, "%s\n", M->format().c_str());
    }

    mir::Module MM;
    mc::Program program;

    if (options.target == Target::kRISCV64) {
        RISCVBackendDriver backendDriver;
        backendDriver.run(*M, MM, program);
    }

    if (options.dumpMIR) {
        fprintf(stderr, "%s\n", MM.format().c_str());
    }

    std::string output = program.format() + '\n';

    if (int error = writeAll(options.outputFile, output)) {
        fprintf(stderr, "%s: error: %s\n", argv[0], strerror(error));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
