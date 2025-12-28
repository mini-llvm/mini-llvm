// SPDX-License-Identifier: MIT

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

Expected<CommandLineParser::Result, CommandLineParser::Error> CommandLineParser::operator()(const std::vector<std::string> &args) {
    std::vector<Argument> result;
    bool isPositional = false;
    size_t n = args.size();
    size_t i = 1;
    while (i < n) {
        std::string arg = args[i++];
        if (isPositional) {
            result.emplace_back(PositionalArgument(arg));
            continue;
        }
        if (arg == "--") {
            result.emplace_back(Separator());
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
                    if (i == n) {
                        return Unexpected(Error(ErrorKind::kMissingValue, arg));
                    }
                    std::string value = args[i++];
                    result.emplace_back(OptionArgument(arg, value));
                    continue;
                }
                result.emplace_back(OptionArgument(arg));
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
            result.emplace_back(OptionArgument(name, value));
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
                        if (i == n) {
                            return Unexpected(Error(ErrorKind::kMissingValue, arg));
                        }
                        std::string value = args[i++];
                        result.emplace_back(OptionArgument(arg, value));
                        break;
                    }
                    result.emplace_back(OptionArgument(arg));
                    break;
                }
                std::string name = arg.substr(0, 2);
                auto option = options_.find(name);
                if (option == options_.end()) {
                    return Unexpected(Error(ErrorKind::kUnrecognizedOption, name));
                }
                if (option->second == OptionKind::kNoValue) {
                    result.emplace_back(OptionArgument(name));
                    arg.erase(arg.begin() + 1);
                    continue;
                }
                std::string value = arg.substr(2);
                result.emplace_back(OptionArgument(name, value));
                break;
            }
            continue;
        }
        result.emplace_back(PositionalArgument(arg));
    }
    return Result(std::move(result));
}
