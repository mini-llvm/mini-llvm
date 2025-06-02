#include "mini-llvm/ir_reader/Lexer.h"

#include <bit>
#include <cctype>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/ir_reader/Token.h"
#include "mini-llvm/utils/HashMap.h"

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
    while (*current_ != '\0') {
        if (*current_ == ' ' || *current_ == '\t' || *current_ == '\n') {
            ++current_;
            continue;
        }
        if (*current_ == ';') {
            ++current_;
            while (*current_ != '\0' && *current_ != '\n') ++current_;
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
            if (isalpha(*current_) || isdigit(*current_) || *current_ == '_' || *current_ == '.') {
                const char *previous = current_;
                while (isalpha(*current_) || isdigit(*current_) || *current_ == '_' || *current_ == '.') {
                    ++current_;
                }
                return {kName, std::string(previous, current_), previous};
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
        const char *previous = current_;
        current_ = previous + 2;
        while (isxdigit(*current_)) ++current_;
        if (*current_ == ':') {
            return {kName, std::string(previous, current_), previous};
        }
        current_ = previous + 2;
        uint64_t value = 0;
        while (isxdigit(*current_)) {
            value = value * 0x10 + ((isdigit(*current_)  ? (*current_ - '0') : ((*current_ | 0x20) - 'a' + 0xa)));
            ++current_;
        }
        return {kNumber, std::bit_cast<int64_t>(value), previous};
    }

    if (isdigit(*current_)) {
        const char *previous = current_;
        uint64_t value = 0;
        while (isdigit(*current_)) {
            value = value * 10 + (*current_ - '0');
            ++current_;
        }
        if (*current_ == ':') {
            return {kName, std::to_string(value), previous};
        } else {
            return {kNumber, std::bit_cast<int64_t>(value), previous};
        }
    }

    if (*current_ == '-') {
        const char *previous = current_;
        ++current_;
        uint64_t value = 0;
        while (isdigit(*current_)) {
            value = value * 10 + (*current_ - '0');
            ++current_;
        }
        value = -value;
        return {kNumber, std::bit_cast<int64_t>(value), previous};
    }

    if ((*current_ == 'c' && *(current_ + 1) == '"') || *current_ == '"') {
        const char *previous = current_;
        while (*current_ == 'c' || *current_ == '"') {
            ++current_;
        }
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
                        if (!isxdigit(ch)) {
                            if (ch == '"') {
                                throw LexException("incomplete escape sequence", current_);
                            } else {
                                throw LexException("invalid character in escape sequence", current_);
                            }
                        }
                        element = element * 0xa + static_cast<int8_t>(isdigit(ch) ? (ch - '0') : ((ch | 0x20) - 'a' + 0xa));
                        ++current_;
                    }
                    elements.push_back(element);
                }
            } else {
                char ch = *current_;
                if (!(0x20 <= ch && ch <= 0x7e)) {
                    throw LexException("unescaped non-printable character", current_);
                }
                int8_t element = static_cast<int8_t>(ch);
                elements.push_back(element);
                ++current_;
            }
        }
        if (*current_ == '\0') {
            throw LexException("missing terminating \" character", current_);
        }
        ++current_;
        if (*previous == 'c') {
            return {kString, std::move(elements), previous};
        }
        std::string name;
        for (int8_t element : elements) {
            name.push_back(static_cast<char>(element));
        }
        return {kName, std::move(name), previous};
    }

    if (isalpha(*current_) || *current_ == '_' || *current_ == '.') {
        const char *previous = current_;
        while (isalpha(*current_) || *current_ == '_' || *current_ == '.' || isdigit(*current_)) {
            ++current_;
        }
        std::string name(previous, current_);

        if (*current_ == ':') {
            return {kName, std::move(name), previous};
        }

        static const HashMap<std::string_view, Token::Kind> kLUT{
            {"define", kDefine},
            {"declare", kDeclare},
            {"global", kGlobal},
            {"constant", kConstant},
            {"private", kPrivate},
            {"internal", kInternal},
            {"external", kExternal},

            {"noinline", kNoInline},
            {"alwaysinline", kAlwaysInline},
            {"readnone", kReadNone},

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
            return {i->second, {}, previous};
        }

        return {kName, std::move(name), previous};
    }

    throw LexException("unexpected character", current_);
}

std::vector<Token> ir::lex(const char *source) {
    Lexer lexer(source);
    std::vector<Token> output;
    do {
        output.push_back(lexer.nextToken());
    } while (output.back().kind != kEOF);
    return output;
}
