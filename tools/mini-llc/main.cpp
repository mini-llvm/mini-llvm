#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ir/Module.h"
#include "ir_parser/ir_parser.h"
#include "ir_parser/Lexer.h"
#include "ir_parser/Parser.h"
#include "mc/Program.h"
#include "opt/ir/passes/VerificationAnalysis.h"
#include "opt/ir/PassManager.h"
#include "targets/riscv/RISCVBackendDriver.h"

namespace {

enum class Target {
    kNone,
    kRISCV64,
};

struct Options {
    const char *inputFile;
    const char *outputFile;
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
            fprintf(stdout, "Usage: %s --target=<target> -o <output-file> <input-file>\n", argv[0]);
            exit(0);

        case CHAR_MAX + 2:
            if (options.target == Target::kNone) {
                if (strcmp(optarg, "riscv64") == 0) {
                    options.target = Target::kRISCV64;
                } else {
                    fprintf(stderr, "%s: error: invalid target '%s'\n", argv[0], optarg);
                    exit(1);
                }
            } else {
                fprintf(stderr, "%s: error: multiple targets\n", argv[0]);
                exit(1);
            }
            break;

        case 'o':
            if (options.outputFile == nullptr) {
                if (*optarg == 0) {
                    fprintf(stderr, "%s: error: output file cannot be empty\n", argv[0]);
                    exit(1);
                }
                options.outputFile = optarg;
            } else {
                fprintf(stderr, "%s: error: multiple output files\n", argv[0]);
                exit(1);
            }
            break;

        default:
            exit(1);
        }
    }

    for (; optind < argc; ++optind) {
        if (options.inputFile == nullptr) {
            if (*argv[optind] == 0) {
                fprintf(stderr, "%s: error: input file cannot be empty\n", argv[0]);
                exit(1);
            }
            options.inputFile = argv[optind];
        } else {
            fprintf(stderr, "%s: error: multiple input files\n", argv[0]);
            exit(1);
        }
    }

    if (options.inputFile == nullptr) {
        fprintf(stderr, "%s: error: no input file\n", argv[0]);
        exit(1);
    }

    if (options.outputFile == nullptr) {
        fprintf(stderr, "%s: error: no output file\n", argv[0]);
        exit(1);
    }

    if (options.target == Target::kNone) {
        fprintf(stderr, "%s: error: no target\n", argv[0]);
        exit(1);
    }

    int inputFd;
    if ((inputFd = open(options.inputFile, O_RDONLY)) == -1) {
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
        fprintf(stderr, "%s:%zu:%zu: error: %s\n", options.inputFile, line, column, message.c_str());
        exit(1);
    }

    try {
        ir::VerificationAnalysis().runOnModule(M);
    } catch (const ir::VerificationException &) {
        fprintf(stderr, "%s: error: input module cannot be verified\n", options.inputFile);
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
    if ((outputFd = open(options.outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
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
