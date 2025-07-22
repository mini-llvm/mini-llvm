#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "mini-llvm/utils/Expected.h"

namespace mini_llvm {

class CommandLineParser {
public:
    class OptionArgument {
    public:
        explicit OptionArgument(std::string name)
            : name_(std::move(name)) {}

        OptionArgument(std::string name, std::string value)
            : name_(std::move(name)), value_(std::move(value)) {}

        const std::string &name() const {
            return name_;
        }

        const std::optional<std::string> &value() const {
            return value_;
        }

    private:
        std::string name_;
        std::optional<std::string> value_;
    };

    class PositionalArgument {
    public:
        explicit PositionalArgument(std::string arg)
            : arg_(std::move(arg)) {}

        const std::string &arg() const {
            return arg_;
        }

    private:
        std::string arg_;
    };

    class Separator {};

    class Argument {
    public:
        explicit Argument(OptionArgument option)
            : arg_(std::move(option)) {}

        explicit Argument(PositionalArgument positionalArg)
            : arg_(std::move(positionalArg)) {}

        explicit Argument(Separator separator)
            : arg_(separator) {}

        const OptionArgument *option() const {
            return std::get_if<OptionArgument>(&arg_);
        }

        const PositionalArgument *positional() const {
            return std::get_if<PositionalArgument>(&arg_);
        }

        const Separator *separator() const {
            return std::get_if<Separator>(&arg_);
        }

    private:
        std::variant<OptionArgument, PositionalArgument, Separator> arg_;
    };

    using iterator = std::vector<Argument>::const_iterator;

    enum class ErrorKind {
        kMissingValue,
        kUnexpectedValue,
        kUnrecognizedOption,
    };

    class Error {
    public:
        Error(ErrorKind kind, std::string optionName)
            : kind_(kind), optionName_(std::move(optionName)) {}

        ErrorKind kind() const {
            return kind_;
        }

        const std::string &optionName() const {
            return optionName_;
        }

    private:
        ErrorKind kind_;
        std::string optionName_;
    };

    void addOption(std::string name);

    Expected<void, Error> operator()(const std::vector<std::string> &args);

    iterator begin() const {
        return args_.begin();
    }

    iterator end() const {
        return args_.end();
    }

    size_t size() const {
        return args_.size();
    }

    bool empty() const {
        return args_.empty();
    }

private:
    enum class OptionKind {
        kNoValue,
        kOptionalValue,
        kRequiredValue,
    };

    std::unordered_map<std::string, OptionKind> options_;
    std::vector<Argument> args_;
};

} // namespace mini_llvm
