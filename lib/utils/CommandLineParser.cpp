#include "mini-llvm/utils/CommandLineParser.h"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/utils/Expected.h"

using namespace mini_llvm;

void CommandLineParser::addOption(std::string name) {
    OptionKind kind = OptionKind::kNoValue;
    if (name.back() == ':') {
        kind = OptionKind::kRequiredValue;
        name.pop_back();
    }
    if (name.back() == ':') {
        kind = OptionKind::kOptionalValue;
        name.pop_back();
    }
    options_.emplace(std::move(name), kind);
}

Expected<void, CommandLineParser::Error> CommandLineParser::parse(int argc, char *argv[]) {
    --argc;
    ++argv;
    bool isPositional = false;
    int i = 0;
    while (i < argc) {
        std::string arg = argv[i++];
        if (isPositional) {
            args_.emplace_back(PositionalArgument(arg));
            continue;
        }
        if (arg == "--") {
            isPositional = true;
            continue;
        }
        if (arg.starts_with("--") && arg != "--") {
            size_t j = arg.find('=');
            if (j == std::string::npos) {
                auto option = options_.find(arg);
                if (option == options_.end()) {
                    return Unexpected(Error(ErrorKind::kUnrecognizedOption, arg));
                }
                if (option->second == OptionKind::kRequiredValue) {
                    if (i == argc) {
                        return Unexpected(Error(ErrorKind::kMissingValue, arg));
                    }
                    std::string value = argv[i++];
                    if (value.starts_with("-") && value != "-") {
                        return Unexpected(Error(ErrorKind::kMissingValue, arg));
                    }
                    args_.emplace_back(OptionArgument(arg, value));
                    continue;
                }
                args_.emplace_back(OptionArgument(arg));
                continue;
            }
            std::string name = arg.substr(0, j);
            auto option = options_.find(name);
            if (option == options_.end()) {
                return Unexpected(Error(ErrorKind::kUnrecognizedOption, name));
            }
            if (option->second == OptionKind::kNoValue) {
                return Unexpected(Error(ErrorKind::kUnexpectedValue, name));
            }
            std::string value = arg.substr(j + 1);
            args_.emplace_back(OptionArgument(name, value));
            continue;
        }
        if (arg.starts_with("-") && arg != "-") {
            while (arg != "-") {
                if (arg.size() == 2) {
                    auto option = options_.find(arg);
                    if (option == options_.end()) {
                        return Unexpected(Error(ErrorKind::kUnrecognizedOption, arg));
                    }
                    if (option->second == OptionKind::kRequiredValue) {
                        if (i == argc) {
                            return Unexpected(Error(ErrorKind::kMissingValue, arg));
                        }
                        std::string value = argv[i++];
                        if (value.starts_with("-") && value != "-") {
                            return Unexpected(Error(ErrorKind::kMissingValue, arg));
                        }
                        args_.emplace_back(OptionArgument(arg, value));
                        break;
                    }
                    args_.emplace_back(OptionArgument(arg));
                    break;
                }
                std::string name = arg.substr(0, 2);
                auto option = options_.find(name);
                if (option == options_.end()) {
                    return Unexpected(Error(ErrorKind::kUnrecognizedOption, name));
                }
                if (option->second == OptionKind::kNoValue) {
                    args_.emplace_back(OptionArgument(name));
                    arg.erase(arg.begin() + 1);
                    continue;
                }
                std::string value = arg.substr(2);
                args_.emplace_back(OptionArgument(name, value));
                break;
            }
            continue;
        }
        args_.emplace_back(PositionalArgument(arg));
    }
    return {};
}
