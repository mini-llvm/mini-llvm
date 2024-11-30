#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/ir_parser/Lexer.h"
#include "mini-llvm/ir_parser/Parser.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"
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
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    Target target;
};

void computeLineColumn(const char *start, const char *location, size_t &line, size_t &column) {
    line = column = 1;
    while (start != location) {
        if (*start == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
        ++start;
    }
}

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

        case CHAR_MAX + 2:
            if (options.target == Target::kNone) {
                Target target = toTarget(optarg);
                if (target != Target::kNone) {
                    options.target = target;
                    break;
                }
                fprintf(stderr, "%s: error: unsupported target '%s'\n", argv[0], optarg);
                exit(1);
            }
            fprintf(stderr, "%s: error: multiple targets\n", argv[0]);
            exit(1);

        case 'o':
            if (options.outputFile.empty()) {
                if (*optarg != '\0') {
                    options.outputFile = optarg;
                    break;
                }
                fprintf(stderr, "%s: error: output file cannot be empty\n", argv[0]);
                exit(1);
            }
            fprintf(stderr, "%s: error: multiple output files\n", argv[0]);
            exit(1);

        default:
            exit(1);
        }
    }

    for (; optind < argc; ++optind) {
        if (options.inputFile.empty()) {
            if (*argv[optind] != '\0') {
                options.inputFile = argv[optind];
                continue;
            }
            fprintf(stderr, "%s: error: input file cannot be empty\n", argv[0]);
            exit(1);
        }
        fprintf(stderr, "%s: error: multiple input files\n", argv[0]);
        exit(1);
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
        if (target != Target::kNone) {
            options.target = target;
        }
        fprintf(stderr, "%s: error: unsupported target '%s'\n", argv[0], targetName);
        exit(1);
    }

    int inputFd;
    if ((inputFd = open(options.inputFile.c_str(), O_RDONLY)) == -1) {
        error(1, errno, "open");
    }

    struct stat sb;
    if (fstat(inputFd, &sb) == -1) {
        error(1, errno, "fstat");
    }

    void *addr;
    if ((addr = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, inputFd, 0)) == MAP_FAILED) {
        error(1, errno, "mmap");
    }

    ir::Module M;
    bool hasError = false;
    const char *location;
    std::string message;
    try {
        M = ir::parseModule(reinterpret_cast<const char *>(addr));
    } catch (const ir::LexException &e) {
        hasError = true;
        location = e.location();
        message = e.message();
    } catch (const ir::ParseException &e) {
        hasError = true;
        location = e.location()->location;
        message = e.message();
    }
    if (hasError) {
        size_t line, column;
        computeLineColumn(reinterpret_cast<const char *>(addr), location, line, column);
        fprintf(stderr, "%s:%zu:%zu: error: %s\n", options.inputFile.c_str(), line, column, message.c_str());
        exit(1);
    }

    try {
        ir::VerificationAnalysis().runOnModule(M);
    } catch (const ir::VerificationException &) {
        fprintf(stderr, "%s: error: input module cannot be verified\n", options.inputFile.c_str());
        exit(1);
    }

    ir::PassManager passManager;
#ifndef NDEBUG
    passManager.setVerifyAfterEach(true);
#endif
    passManager.run(M);

    mc::Program program;

    if (options.target == Target::kRISCV64) {
        RISCVBackendDriver backendDriver;
        program = backendDriver.run(M);
    }

    std::string output = program.format() + '\n';

    int outputFd;
    if ((outputFd = open(options.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
        error(1, errno, "open");
    }

    if (write(outputFd, output.data(), output.size()) == -1) {
        error(1, errno, "write");
    }

    close(outputFd);

    munmap(addr, sb.st_size);
    close(inputFd);

    exit(0);
}
