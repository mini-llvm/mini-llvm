// SPDX-License-Identifier: MIT

#include "mini-llvm/ir_reader/Lexer.h"

#include <bit>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/ir_reader/Token.h"
#include "mini-llvm/utils/Ascii.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Unicode.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

using enum Token::Kind;

class Lexer::Impl {
public:
    explicit Impl(const char *current) : current_(current) {}

    std::optional<Token> lastToken() {
        return lastToken_;
    }

    Token nextToken() {
        Token token = nextTokenImpl();
        lastToken_ = token;
        return token;
    }

private:
    const char *current_;
    std::optional<Token> lastToken_;

    Token nextTokenImpl() {
        while (*current_ != '\0') {
            switch (*current_) {
            case ' ':
            case '\t':
            case '\v':
            case '\f':
            case '\n':
            case '\r':
                ++current_;
                continue;

            case ';':
                ++current_;
                while (*current_ != '\0' && *current_ != '\n') {
                    ++current_;
                }
                continue;
            }
            break;
        }

        if (*current_ == '\0') {
            return {kEOF, {}, current_};
        }

        if (lastToken()) {
            Token::Kind kind = lastToken()->kind;
            if (kind == kAt || kind == kPercent) {
                if (isAsciiLetter(*current_) || isAsciiDigit(*current_) || *current_ == '$' || *current_ == '.' || *current_ == '_') {
                    const char *start = current_;
                    while (isAsciiLetter(*current_) || isAsciiDigit(*current_) || *current_ == '$' || *current_ == '.' || *current_ == '_') {
                        ++current_;
                    }
                    return {kName, std::string(start, current_), start};
                }
            }
        }

        static const HashMap<char, Token::Kind> kLUT{
            {',', kComma},
            {':', kColon},
            {'=', kEqual},
            {'@', kAt},
            {'%', kPercent},
            {'(', kLeftParen},
            {')', kRightParen},
            {'[', kLeftBracket},
            {']', kRightBracket},
            {'{', kLeftBrace},
            {'}', kRightBrace},
        };

        if (auto i = kLUT.find(*current_); i != kLUT.end()) {
            return {i->second, {}, current_++};
        }

        if (*current_ == '0' && (*(current_ + 1) == 'X' || *(current_ + 1) == 'x')) {
            const char *start = current_;
            ++current_;
            ++current_;
            uint64_t value = 0;
            while (isAsciiHexDigit(*current_)) {
                value = value * 0x10 + ((isAsciiDigit(*current_) ? (*current_ - '0') : (toAsciiLower(*current_) - 'a' + 0xa)));
                ++current_;
            }
            return {kNumber, std::bit_cast<int64_t>(value), start};
        }

        if (isAsciiDigit(*current_)) {
            const char *start = current_;
            uint64_t value = 0;
            while (isAsciiDigit(*current_)) {
                value = value * 10 + (*current_ - '0');
                ++current_;
            }
            if (*current_ == ':') {
                return {kName, std::to_string(value), start};
            } else {
                return {kNumber, std::bit_cast<int64_t>(value), start};
            }
        }

        if (*current_ == '-') {
            const char *start = current_;
            ++current_;
            uint64_t value = 0;
            while (isAsciiDigit(*current_)) {
                value = value * 10 + (*current_ - '0');
                ++current_;
            }
            value = -value;
            return {kNumber, std::bit_cast<int64_t>(value), start};
        }

        if ((*current_ == 'c' && *(current_ + 1) == '"') || *current_ == '"') {
            const char *start = current_;
            if (*current_ == 'c') {
                ++current_;
            }
            ++current_;
            std::vector<int8_t> elements;
            while (*current_ != '\0' && *current_ != '"') {
                if (*current_ == '\\') {
                    ++current_;
                    if (*current_ == '\\') {
                        ++current_;
                        elements.push_back(static_cast<int8_t>('\\'));
                    } else {
                        int8_t element = 0;
                        for (int i = 0; i < 2; ++i) {
                            char ch = *current_;
                            if (ch == 0) {
                                throw LexException("missing terminating \" character", current_);
                            }
                            if (!isAsciiHexDigit(ch)) {
                                if (ch == '"') {
                                    throw LexException("incomplete escape sequence", current_);
                                } else {
                                    throw LexException("invalid character in escape sequence", current_);
                                }
                            }
                            element = element * 0x10 + static_cast<int8_t>(isAsciiDigit(ch) ? (ch - '0') : (toAsciiLower(ch) - 'a' + 0xa));
                            ++current_;
                        }
                        elements.push_back(element);
                    }
                } else {
                    const char *next = current_;
                    char32_t cp = decodeUtf8(next);
                    if (cp == static_cast<char32_t>(-1)) {
                        throw LexException("invalid UTF-8 character", current_);
                    }
                    while (current_ < next) {
                        int8_t element = static_cast<int8_t>(*current_);
                        elements.push_back(element);
                        ++current_;
                    }
                }
            }
            if (*current_ == '\0') {
                throw LexException("missing terminating \" character", current_);
            }
            ++current_;
            if (*start == 'c') {
                return {kString, std::move(elements), start};
            }
            std::string name;
            for (int8_t element : elements) {
                name.push_back(static_cast<char>(element));
            }
            return {kName, std::move(name), start};
        }

        if (isAsciiLetter(*current_) || *current_ == '$' || *current_ == '.' || *current_ == '_') {
            const char *start = current_;
            while (isAsciiLetter(*current_) || *current_ == '$' || *current_ == '.' || *current_ == '_' || isAsciiDigit(*current_)) {
                ++current_;
            }
            std::string name(start, current_);

            if (*current_ == ':') {
                return {kName, std::move(name), start};
            }

            static const HashMap<std::string_view, Token::Kind> kLUT{
                {"define", kDefine},
                {"declare", kDeclare},
                {"global", kGlobal},
                {"constant", kConstant},
                {"private", kPrivate},
                {"internal", kInternal},
                {"external", kExternal},

                {"alwaysinline", kAlwaysInline},
                {"noinline", kNoInline},
                {"readnone", kReadNone},
                {"readonly", kReadOnly},

                {"i1", kI1},
                {"i8", kI8},
                {"i16", kI16},
                {"i32", kI32},
                {"i64", kI64},
                {"float", kFloat},
                {"double", kDouble},
                {"ptr", kPtr},
                {"label", kLabel},
                {"void", kVoid},

                {"true", kTrue},
                {"false", kFalse},
                {"null", kNull},
                {"zeroinitializer", kZeroInitializer},
                {"poison", kPoison},

                {"add", kAdd},
                {"alloca", kAlloca},
                {"and", kAnd},
                {"ashr", kASHR},
                {"bitcast", kBitCast},
                {"br", kBr},
                {"call", kCall},
                {"fadd", kFAdd},
                {"fcmp", kFCmp},
                {"fdiv", kFDiv},
                {"fmul", kFMul},
                {"fneg", kFNeg},
                {"fpext", kFPExt},
                {"fptosi", kFPToSI},
                {"fptoui", kFPToUI},
                {"fptrunc", kFPTrunc},
                {"frem", kFRem},
                {"fsub", kFSub},
                {"getelementptr", kGetElementPtr},
                {"icmp", kICmp},
                {"inttoptr", kIntToPtr},
                {"load", kLoad},
                {"lshr", kLSHR},
                {"mul", kMul},
                {"or", kOr},
                {"phi", kPhi},
                {"ptrtoint", kPtrToInt},
                {"ret", kRet},
                {"sdiv", kSDiv},
                {"select", kSelect},
                {"sext", kSExt},
                {"shl", kSHL},
                {"sitofp", kSIToFP},
                {"srem", kSRem},
                {"store", kStore},
                {"sub", kSub},
                {"trunc", kTrunc},
                {"udiv", kUDiv},
                {"uitofp", kUIToFP},
                {"urem", kURem},
                {"xor", kXor},
                {"zext", kZExt},

                {"to", kTo},

                {"eq", kEQ},
                {"ne", kNE},
                {"slt", kSLT},
                {"sgt", kSGT},
                {"sle", kSLE},
                {"sge", kSGE},
                {"ult", kULT},
                {"ugt", kUGT},
                {"ule", kULE},
                {"uge", kUGE},

                {"oeq", kOEQ},
                {"one", kONE},
                {"olt", kOLT},
                {"ogt", kOGT},
                {"ole", kOLE},
                {"oge", kOGE},

                {"x", kX},

                {"...", kEllipsis},
            };

            if (auto i = kLUT.find(name); i != kLUT.end()) {
                return {i->second, {}, start};
            }

            return {kName, std::move(name), start};
        }

        throw LexException("unexpected character", current_);
    }
};

Lexer::Lexer(const char *current)
    : impl_(std::make_unique<Impl>(current)) {}

Lexer::~Lexer() = default;

Lexer::Lexer(Lexer &&) noexcept = default;

Lexer &Lexer::operator=(Lexer &&) noexcept = default;

std::optional<Token> Lexer::lastToken() {
    return impl_->lastToken();
}

Token Lexer::nextToken() {
    return impl_->nextToken();
}

std::vector<Token> ir::lex(const char *source) {
    Lexer lexer(source);
    std::vector<Token> output;
    do {
        output.push_back(lexer.nextToken());
    } while (output.back().kind != kEOF);
    return output;
}
