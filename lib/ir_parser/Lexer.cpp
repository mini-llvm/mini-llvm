#include "mini-llvm/ir_parser/Lexer.h"

#include <bit>
#include <cctype>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir_parser/Token.h"

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
    while (*cursor_ != 0) {
        if (*cursor_ == ' ' || *cursor_ == '\t' || *cursor_ == '\n') {
            ++cursor_;
            continue;
        }
        if (*cursor_ == ';') {
            ++cursor_;
            while (*cursor_ != 0 && *cursor_ != '\n') ++cursor_;
            continue;
        }
        break;
    }

    if (*cursor_ == 0) {
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

    if (*cursor_ == ',') {
        const char *start = cursor_;
        ++cursor_;
        return {kComma, {}, start};
    }
    if (*cursor_ == ':') {
        const char *start = cursor_;
        ++cursor_;
        return {kColon, {}, start};
    }
    if (*cursor_ == '=') {
        const char *start = cursor_;
        ++cursor_;
        return {kEqual, {}, start};
    }
    if (*cursor_ == '@') {
        const char *start = cursor_;
        ++cursor_;
        return {kAt, {}, start};
    }
    if (*cursor_ == '%') {
        const char *start = cursor_;
        ++cursor_;
        return {kPercent, {}, start};
    }
    if (*cursor_ == '(') {
        const char *start = cursor_;
        ++cursor_;
        return {kLeftParen, {}, start};
    }
    if (*cursor_ == ')') {
        const char *start = cursor_;
        ++cursor_;
        return {kRightParen, {}, start};
    }
    if (*cursor_ == '[') {
        const char *start = cursor_;
        ++cursor_;
        return {kLeftBracket, {}, start};
    }
    if (*cursor_ == ']') {
        const char *start = cursor_;
        ++cursor_;
        return {kRightBracket, {}, start};
    }
    if (*cursor_ == '{') {
        const char *start = cursor_;
        ++cursor_;
        return {kLeftBrace, {}, start};
    }
    if (*cursor_ == '}') {
        const char *start = cursor_;
        ++cursor_;
        return {kRightBrace, {}, start};
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
        while (*cursor_ != 0 && *cursor_ != '"') {
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
                            throw LexException(cursor_, "unterminated string");
                        }
                        if (!isxdigit(ch)) {
                            if (ch == '"') {
                                throw LexException(cursor_, "incomplete escape sequence");
                            } else {
                                throw LexException(cursor_, "invalid character in escape sequence");
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
                    throw LexException(cursor_, "only printable ASCII characters are permitted");
                }
                int8_t element = static_cast<int8_t>(ch);
                elements.push_back(element);
                ++cursor_;
            }
        }
        if (*cursor_ == 0) {
            throw LexException(cursor_, "unterminated string");
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

        if (name == "define") return {kDefine, {}, start};
        if (name == "declare") return {kDeclare, {}, start};
        if (name == "global") return {kGlobal, {}, start};
        if (name == "internal") return {kInternal, {}, start};
        if (name == "external") return {kExternal, {}, start};

        if (name == "noinline") return {kNoInline, {}, start};
        if (name == "alwaysinline") return {kAlwaysInline, {}, start};

        if (name == "i1") return {kI1, {}, start};
        if (name == "i8") return {kI8, {}, start};
        if (name == "i16") return {kI16, {}, start};
        if (name == "i32") return {kI32, {}, start};
        if (name == "i64") return {kI64, {}, start};
        if (name == "float") return {kFloat, {}, start};
        if (name == "double") return {kDouble, {}, start};
        if (name == "ptr") return {kPtr, {}, start};
        if (name == "label") return {kLabel, {}, start};
        if (name == "void") return {kVoid, {}, start};

        if (name == "true") return {kTrue, {}, start};
        if (name == "false") return {kFalse, {}, start};
        if (name == "null") return {kNull, {}, start};
        if (name == "zeroinitializer") return {kZeroInitializer, {}, start};
        if (name == "poison") return {kPoison, {}, start};

        if (name == "add") return {kAdd, {}, start};
        if (name == "alloca") return {kAlloca, {}, start};
        if (name == "and") return {kAnd, {}, start};
        if (name == "ashr") return {kASHR, {}, start};
        if (name == "bitcast") return {kBitCast, {}, start};
        if (name == "br") return {kBr, {}, start};
        if (name == "call") return {kCall, {}, start};
        if (name == "fadd") return {kFAdd, {}, start};
        if (name == "fcmp") return {kFCmp, {}, start};
        if (name == "fdiv") return {kFDiv, {}, start};
        if (name == "fmul") return {kFMul, {}, start};
        if (name == "fneg") return {kFNeg, {}, start};
        if (name == "fpext") return {kFPExt, {}, start};
        if (name == "fptosi") return {kFPToSI, {}, start};
        if (name == "fptoui") return {kFPToUI, {}, start};
        if (name == "fptrunc") return {kFPTrunc, {}, start};
        if (name == "frem") return {kFRem, {}, start};
        if (name == "fsub") return {kFSub, {}, start};
        if (name == "getelementptr") return {kGetElementPtr, {}, start};
        if (name == "icmp") return {kICmp, {}, start};
        if (name == "load") return {kLoad, {}, start};
        if (name == "lshr") return {kLSHR, {}, start};
        if (name == "mul") return {kMul, {}, start};
        if (name == "or") return {kOr, {}, start};
        if (name == "phi") return {kPhi, {}, start};
        if (name == "ret") return {kRet, {}, start};
        if (name == "sdiv") return {kSDiv, {}, start};
        if (name == "select") return {kSelect, {}, start};
        if (name == "sext") return {kSExt, {}, start};
        if (name == "shl") return {kSHL, {}, start};
        if (name == "sitofp") return {kSIToFP, {}, start};
        if (name == "srem") return {kSRem, {}, start};
        if (name == "store") return {kStore, {}, start};
        if (name == "sub") return {kSub, {}, start};
        if (name == "trunc") return {kTrunc, {}, start};
        if (name == "udiv") return {kUDiv, {}, start};
        if (name == "uitofp") return {kUIToFP, {}, start};
        if (name == "urem") return {kURem, {}, start};
        if (name == "xor") return {kXor, {}, start};
        if (name == "zext") return {kZExt, {}, start};

        if (name == "to") return {kTo, {}, start};

        if (name == "eq") return {kEQ, {}, start};
        if (name == "ne") return {kNE, {}, start};
        if (name == "slt") return {kSLT, {}, start};
        if (name == "sgt") return {kSGT, {}, start};
        if (name == "sle") return {kSLE, {}, start};
        if (name == "sge") return {kSGE, {}, start};
        if (name == "ult") return {kULT, {}, start};
        if (name == "ugt") return {kUGT, {}, start};
        if (name == "ule") return {kULE, {}, start};
        if (name == "uge") return {kUGE, {}, start};

        if (name == "oeq") return {kOEQ, {}, start};
        if (name == "one") return {kONE, {}, start};
        if (name == "olt") return {kOLT, {}, start};
        if (name == "ogt") return {kOGT, {}, start};
        if (name == "ole") return {kOLE, {}, start};
        if (name == "oge") return {kOGE, {}, start};

        if (name == "x") return {kX, {}, start};

        if (name == "...") return {kEllipsis, {}, start};

        return {kName, std::move(name), start};
    }

    throw LexException(cursor_, "unexpected character");
}

Lexer::operator std::vector<Token>() {
    std::vector<Token> output;
    do {
        output.push_back(nextToken());
    } while (output.back().kind != kEOF);
    return output;
}
