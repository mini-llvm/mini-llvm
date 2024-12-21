#include "mini-llvm/ir_parser/Lexer.h"

#include <bit>
#include <cctype>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mini-llvm/ir_parser/Token.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

using enum Token::Kind;

std::optional<Token> Lexer::lastToken() {
    return lastToken_;
}

Token Lexer::nextToken() {
    Token token = nextTokenImpl();
    lastToken_ = token;
    return token;
}

Token Lexer::nextTokenImpl() {
    while (*cursor_ != '\0') {
        if (*cursor_ == ' ' || *cursor_ == '\t' || *cursor_ == '\n') {
            ++cursor_;
            continue;
        }
        if (*cursor_ == ';') {
            ++cursor_;
            while (*cursor_ != '\0' && *cursor_ != '\n') ++cursor_;
            continue;
        }
        break;
    }

    if (*cursor_ == '\0') {
        return {kEOF, {}, cursor_};
    }

    if (lastToken().has_value()) {
        Token::Kind kind = lastToken().value().kind;
        if (kind == kAt || kind == kPercent) {
            if (isalpha(*cursor_) || isdigit(*cursor_) || *cursor_ == '_' || *cursor_ == '.') {
                const char *start = cursor_;
                while (isalpha(*cursor_) || isdigit(*cursor_) || *cursor_ == '_' || *cursor_ == '.') {
                    ++cursor_;
                }
                return {kName, std::string(start, cursor_), start};
            }
        }
    }

    const std::unordered_map<char, Token::Kind> kLUT{
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

    if (auto i = kLUT.find(*cursor_); i != kLUT.end()) {
        const char *start = cursor_;
        ++cursor_;
        return {i->second, {}, start};
    }

    if (*cursor_ == '0' && (*(cursor_ + 1) == 'X' || *(cursor_ + 1) == 'x')) {
        const char *start = cursor_;
        cursor_ = start + 2;
        while (isxdigit(*cursor_)) ++cursor_;
        if (*cursor_ == ':') {
            return {kName, std::string(start, cursor_), start};
        }
        cursor_ = start + 2;
        uint64_t value = 0;
        while (isxdigit(*cursor_)) {
            value = value * 0x10 + ((isdigit(*cursor_)  ? (*cursor_ - '0') : ((*cursor_ | 0x20) - 'a' + 0xa)));
            ++cursor_;
        }
        return {kNumber, std::bit_cast<int64_t>(value), start};
    }

    if (isdigit(*cursor_)) {
        const char *start = cursor_;
        uint64_t value = 0;
        while (isdigit(*cursor_)) {
            value = value * 10 + (*cursor_ - '0');
            ++cursor_;
        }
        if (*cursor_ == ':') {
            return {kName, std::to_string(value), start};
        } else {
            return {kNumber, std::bit_cast<int64_t>(value), start};
        }
    }

    if (*cursor_ == '-') {
        const char *start = cursor_;
        ++cursor_;
        uint64_t value = 0;
        while (isdigit(*cursor_)) {
            value = value * 10 + (*cursor_ - '0');
            ++cursor_;
        }
        value = -value;
        return {kNumber, std::bit_cast<int64_t>(value), start};
    }

    if (*cursor_ == 'c' && *(cursor_ + 1) == '"') {
        const char *start = cursor_;
        cursor_ += 2;
        std::vector<int8_t> elements;
        while (*cursor_ != '\0' && *cursor_ != '"') {
            if (*cursor_ == '\\') {
                ++cursor_;
                if (*cursor_ == '\\') {
                    ++cursor_;
                    elements.push_back(static_cast<int8_t>('\\'));
                } else {
                    int8_t element = 0;
                    for (int i = 0; i < 2; ++i) {
                        char ch = *cursor_;
                        if (ch == 0) {
                            throw LexException("unterminated string", cursor_);
                        }
                        if (!isxdigit(ch)) {
                            if (ch == '"') {
                                throw LexException("incomplete escape sequence", cursor_);
                            } else {
                                throw LexException("invalid character in escape sequence", cursor_);
                            }
                        }
                        element = element * 0xa + static_cast<int8_t>(isdigit(ch) ? (ch - '0') : ((ch | 0x20) - 'a' + 0xa));
                        ++cursor_;
                    }
                    elements.push_back(element);
                }
            } else {
                char ch = *cursor_;
                if (!(0x20 <= ch && ch <= 0x7e)) {
                    throw LexException("only characters between \\x20 and \\x7e are permitted", cursor_);
                }
                int8_t element = static_cast<int8_t>(ch);
                elements.push_back(element);
                ++cursor_;
            }
        }
        if (*cursor_ == '\0') {
            throw LexException("unterminated string", cursor_);
        }
        ++cursor_;
        return {kString, std::move(elements), start};
    }

    if (isalpha(*cursor_) || *cursor_ == '_' || *cursor_ == '.') {
        const char *start = cursor_;
        while (isalpha(*cursor_) || *cursor_ == '_' || *cursor_ == '.' || isdigit(*cursor_)) {
            ++cursor_;
        }
        std::string name(start, cursor_);

        if (*cursor_ == ':') return {kName, std::move(name), start};

        const std::unordered_map<std::string_view, Token::Kind> kLUT{
            {"define", kDefine},
            {"declare", kDeclare},
            {"global", kGlobal},
            {"internal", kInternal},
            {"external", kExternal},

            {"noinline", kNoInline},
            {"alwaysinline", kAlwaysInline},

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

    throw LexException("unexpected character", cursor_);
}

std::vector<Token> ir::lex(const char *source) {
    Lexer lexer(source);
    std::vector<Token> output;
    do {
        output.push_back(lexer.nextToken());
    } while (output.back().kind != kEOF);
    return output;
}
