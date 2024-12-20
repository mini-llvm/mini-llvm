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
#include "mini-llvm/ir_parser/IRParser.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
#include "mini-llvm/utils/FileSystem.h"
#include "mini-llvm/utils/ProcessorDetection.h"
#include "mini-llvm/utils/Status.h"
#include "mini-llvm/utils/SystemError.h"

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
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    Target target;
};

} // namespace

int main(int argc, char *argv[]) {
    Options options{};

    const char *shortOpts = "o:";
    struct option longOpts[] = {
        {"help", no_argument, nullptr, CHAR_MAX + 1},
        {"target", required_argument, nullptr, CHAR_MAX + 2},
        {nullptr, 0, nullptr, 0},
    };

    int opt;
    while ((opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1) {
        switch (opt) {
        case CHAR_MAX + 1:
            fprintf(stdout, "Usage: %s [--target=<target>] [-o <output-file>] <input-file>\n", argv[0]);
            exit(0);

        case CHAR_MAX + 2: {
            if (options.target != Target::kNone) {
                fprintf(stderr, "%s: error: multiple targets\n", argv[0]);
                exit(1);
            }
            Target target = toTarget(optarg);
            if (target == Target::kNone) {
                fprintf(stderr, "%s: error: unsupported target '%s'\n", argv[0], optarg);
                exit(1);
            }
            options.target = target;
            break;
        }

        case 'o':
            if (!options.outputFile.empty()) {
                fprintf(stderr, "%s: error: multiple output files\n", argv[0]);
                exit(1);
            }
            if (*optarg == '\0') {
                fprintf(stderr, "%s: error: output file cannot be empty\n", argv[0]);
                exit(1);
            }
            options.outputFile = optarg;
            break;

        default:
            exit(1);
        }
    }

    for (; optind < argc; ++optind) {
        if (!options.inputFile.empty()) {
            fprintf(stderr, "%s: error: multiple input files\n", argv[0]);
            exit(1);
        }
        if (*argv[optind] == '\0') {
            fprintf(stderr, "%s: error: input file cannot be empty\n", argv[0]);
            exit(1);
        }
        options.inputFile = argv[optind];
    }

    if (options.inputFile.empty()) {
        fprintf(stderr, "%s: error: no input file\n", argv[0]);
        exit(1);
    }

    if (options.outputFile.empty()) {
        options.outputFile = std::filesystem::path(options.inputFile).replace_extension(".s");
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
            fprintf(stderr, "%s: error: native target '%s' not supported\n", argv[0], targetName);
            exit(1);
        }
        options.target = target;
    }

    Expected<std::string, SystemError> input = readAll(options.inputFile);
    if (!input.ok()) {
        fprintf(stderr, "%s: error: %s: %s\n", argv[0], input.error().call, strerror(input.error().error));
        exit(1);
    }

    std::vector<Diagnostic> diags;
    std::optional<ir::Module> M = ir::parseModule(input.value().c_str(), diags);
    if (!M.has_value()) {
        for (Diagnostic &diag : diags) {
            diag.file = options.inputFile;
        }
        for (const Diagnostic &diag : diags) {
            fprintf(stderr, "%s\n", diag.format().c_str());
        }
        exit(1);
    }

    ir::PassManager passManager;
#ifndef NDEBUG
    passManager.setVerifyAfterEach(true);
#endif
    passManager.run(M.value());

    mc::Program program;

    if (options.target == Target::kRISCV64) {
        RISCVBackendDriver backendDriver;
        program = backendDriver.run(M.value());
    }

    std::string output = program.format() + '\n';

    if (Status status = writeAll(options.outputFile, output); !status.ok()) {
        fprintf(stderr, "%s: error: %s: %s\n", argv[0], status.error().call, strerror(status.error().error));
        exit(1);
    }

    exit(0);
}
