#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "mini-llvm/utils/Expected.h"

namespace mini_llvm {

class CommandLineParser {
    enum class OptionKind {
        kNoValue,
        kOptionalValue,
        kRequiredValue,
    };

    class OptionArgument {
    public:
        explicit OptionArgument(std::string name)
            : name_(std::move(name)) {}

        OptionArgument(std::string name, std::string value)
            : name_(std::move(name)), value_(std::move(value)) {}

        const std::string &name() const {
            return name_;
        }

        bool hasValue() const {
            return static_cast<bool>(value_);
        }

        const std::string &value() const {
            return *value_;
        }

        std::string valueOr(std::string value) const {
            return value_.value_or(std::move(value));
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

public:
    class Argument {
    public:
        explicit Argument(OptionArgument option)
            : arg_(std::move(option)) {}

        explicit Argument(PositionalArgument positionalArg)
            : arg_(std::move(positionalArg)) {}

        bool isOption() const {
            return std::holds_alternative<OptionArgument>(arg_);
        }

        bool isPositional() const {
            return std::holds_alternative<PositionalArgument>(arg_);
        }

        const std::string &name() const {
            return std::get<OptionArgument>(arg_).name();
        }

        bool hasValue() const {
            return std::get<OptionArgument>(arg_).hasValue();
        }

        const std::string &value() const {
            return std::get<OptionArgument>(arg_).value();
        }

        std::string valueOr(std::string value) const {
            return std::get<OptionArgument>(arg_).valueOr(std::move(value));
        }

        const std::string &arg() const {
            return std::get<PositionalArgument>(arg_).arg();
        }

    private:
        std::variant<OptionArgument, PositionalArgument> arg_;
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

    Expected<void, Error> parse(int argc, char *argv[]);

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
    std::unordered_map<std::string, OptionKind> options_;
    std::vector<Argument> args_;
};

} // namespace mini_llvm
