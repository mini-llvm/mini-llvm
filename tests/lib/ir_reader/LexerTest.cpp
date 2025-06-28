#include <cstdint>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir_reader/Lexer.h"
#include "mini-llvm/ir_reader/Token.h"

using namespace mini_llvm::ir;

using enum Token::Kind;

TEST(LexerTest, Empty) {
    const char *input = "";

    std::vector<Token> expectedOutput{
        {kEOF, {}, input},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, Comment) {
    const char *input = R"(
; Comment
@test = ; Comment
; Comment
; Comment
global i32 42 ; Comment
; Comment
)";

    std::vector<Token> expectedOutput{
        {kAt,     {},     input + 11},
        {kName,   "test", input + 12},
        {kEqual,  {},     input + 17},
        {kGlobal, {},     input + 49},
        {kI32,    {},     input + 56},
        {kNumber, 42,     input + 60},
        {kEOF,    {},     input + 83},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberPositive) {
    const char *input = "42";

    std::vector<Token> expectedOutput{
        {kNumber, 42, input + 0},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberNegative) {
    const char *input = "-42";

    std::vector<Token> expectedOutput{
        {kNumber, -42, input + 0},
        {kEOF, {}, input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberZero) {
    const char *input = "0";

    std::vector<Token> expectedOutput{
        {kNumber, 0, input + 0},
        {kEOF, {}, input + 1},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberSignedMax) {
    const char *input = "9223372036854775807";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::max(), input + 0},
        {kEOF, {}, input + 19},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberSignedMin) {
    const char *input = "-9223372036854775808";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::min(), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, NumberUnsignedMax) {
    const char *input = "18446744073709551615";

    std::vector<Token> expectedOutput{
        {kNumber, static_cast<int64_t>(std::numeric_limits<uint64_t>::max()), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, HexNumberSignedMax) {
    const char *input = "0x7fffffffffffffff";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::max(), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, HexNumberSignedMin) {
    const char *input = "0x8000000000000000";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::min(), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, HexNumberUnsignedMax) {
    const char *input = "0xffffffffffffffff";

    std::vector<Token> expectedOutput{
        {kNumber, static_cast<int64_t>(std::numeric_limits<uint64_t>::max()), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, Label) {
    const char *input = "AZaz09._:";

    std::vector<Token> expectedOutput{
        {kName,  "AZaz09._", input + 0},
        {kColon, {},       input + 8},
        {kEOF,   {},       input + 9},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LabelNumber) {
    const char *input = "42:";

    std::vector<Token> expectedOutput{
        {kName, "42", input + 0},
        {kColon, {},  input + 2},
        {kEOF,   {},  input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LabelUnderscore) {
    const char *input = "_:";

    std::vector<Token> expectedOutput{
        {kName, "_", input + 0},
        {kColon, {}, input + 1},
        {kEOF,   {}, input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LabelDot) {
    const char *input = ".:";

    std::vector<Token> expectedOutput{
        {kName, ".", input + 0},
        {kColon, {}, input + 1},
        {kEOF,   {}, input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LabelReserved) {
    const char *input = "define:";

    std::vector<Token> expectedOutput{
        {kName, "define", input + 0},
        {kColon, {},      input + 6},
        {kEOF,   {},      input + 7},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LocalIdentifier) {
    const char *input = "%AZaz09._";

    std::vector<Token> expectedOutput{
        {kPercent, {},         input + 0},
        {kName,    "AZaz09._", input + 1},
        {kEOF,     {},         input + 9},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LocalIdentifierNumber) {
    const char *input = "%42";

    std::vector<Token> expectedOutput{
        {kPercent, {},   input + 0},
        {kName,    "42", input + 1},
        {kEOF,     {},   input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LocalIdentifierUnderscore) {
    const char *input = "%_";

    std::vector<Token> expectedOutput{
        {kPercent, {},  input + 0},
        {kName,    "_", input + 1},
        {kEOF,     {},  input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LocalIdentifierDot) {
    const char *input = "%.";

    std::vector<Token> expectedOutput{
        {kPercent, {},  input + 0},
        {kName,    ".", input + 1},
        {kEOF,     {},  input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, LocalIdentifierReserved) {
    const char *input = "%define";

    std::vector<Token> expectedOutput{
        {kPercent, {},       input + 0},
        {kName,    "define", input + 1},
        {kEOF,     {},       input + 7},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, GlobalIdentifier) {
    const char *input = "@AZaz09._";

    std::vector<Token> expectedOutput{
        {kAt,   {},         input + 0},
        {kName, "AZaz09._", input + 1},
        {kEOF,  {},         input + 9},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, GlobalIdentifierNumber) {
    const char *input = "@42";

    std::vector<Token> expectedOutput{
        {kAt,   {},   input + 0},
        {kName, "42", input + 1},
        {kEOF,  {},   input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, GlobalIdentifierUnderscore) {
    const char *input = "@_";

    std::vector<Token> expectedOutput{
        {kAt,   {},  input + 0},
        {kName, "_", input + 1},
        {kEOF,  {},  input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, GlobalIdentifierDot) {
    const char *input = "@.";

    std::vector<Token> expectedOutput{
        {kAt,   {},  input + 0},
        {kName, ".", input + 1},
        {kEOF,  {},  input + 2},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, GlobalIdentifierReserved) {
    const char *input = "@define";

    std::vector<Token> expectedOutput{
        {kAt,   {},       input + 0},
        {kName, "define", input + 1},
        {kEOF,  {},       input + 7},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, String) {
    const char *input = R"(c" ~AZaz\\09\00\1F\7F\FF")";

    std::vector<Token> expectedOutput{
        {kString, std::vector<int8_t>{32, 126, 65, 90, 97, 122, 92, 48, 57, 0, 31, 127, -1}, input + 0},
        {kEOF, {}, input + 25},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}
