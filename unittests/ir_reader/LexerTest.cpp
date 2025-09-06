// SPDX-License-Identifier: MIT

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir_reader/Lexer.h"
#include "mini-llvm/ir_reader/Token.h"
#include "mini-llvm/utils/BigInteger.h"

using enum mini_llvm::ir::Token::Kind;

namespace mini_llvm::ir {

TEST(LexerTest, Empty) {
    const char *input = "";

    std::vector<Token> expected{
        {kEOF, {}, input},
    };

    EXPECT_EQ(lex(input), expected);
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

    std::vector<Token> expected{
        {kAt, {}, input + 11},
        {kName, "test", input + 12},
        {kEqual, {}, input + 17},
        {kGlobal, {}, input + 49},
        {kI32, {}, input + 56},
        {kNumber, BigInteger("42"), input + 60},
        {kEOF, {}, input + 83},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberPositive) {
    const char *input = "42";

    std::vector<Token> expected{
        {kNumber, BigInteger("42"), input + 0},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberNegative) {
    const char *input = "-42";

    std::vector<Token> expected{
        {kNumber, BigInteger("-42"), input + 0},
        {kEOF, {}, input + 3},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberZero) {
    const char *input = "0";

    std::vector<Token> expected{
        {kNumber, BigInteger("0"), input + 0},
        {kEOF, {}, input + 1},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberSignedMax) {
    const char *input = "9223372036854775807";

    std::vector<Token> expected{
        {kNumber, BigInteger("9223372036854775807"), input + 0},
        {kEOF, {}, input + 19},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberSignedMin) {
    const char *input = "-9223372036854775808";

    std::vector<Token> expected{
        {kNumber, BigInteger("-9223372036854775808"), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, NumberUnsignedMax) {
    const char *input = "18446744073709551615";

    std::vector<Token> expected{
        {kNumber, BigInteger("18446744073709551615"), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, HexNumberSignedMax) {
    const char *input = "0x7fffffffffffffff";

    std::vector<Token> expected{
        {kNumber, BigInteger("7fffffffffffffff", 16), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, HexNumberSignedMin) {
    const char *input = "0x8000000000000000";

    std::vector<Token> expected{
        {kNumber, BigInteger("8000000000000000", 16), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, HexNumberUnsignedMax) {
    const char *input = "0xffffffffffffffff";

    std::vector<Token> expected{
        {kNumber, BigInteger("ffffffffffffffff", 16), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, Label) {
    const char *input = "AZaz09$._:";

    std::vector<Token> expected{
        {kName, "AZaz09$._", input + 0},
        {kColon, {}, input + 9},
        {kEOF, {}, input + 10},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelNumber) {
    const char *input = "42:";

    std::vector<Token> expected{
        {kName, "42", input + 0},
        {kColon, {}, input + 2},
        {kEOF, {}, input + 3},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelNumberHuge) {
    const char *input = "12345678901234567890:";

    std::vector<Token> expected{
        {kName, "12345678901234567890", input + 0},
        {kColon, {}, input + 20},
        {kEOF, {}, input + 21},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelDollar) {
    const char *input = "$:";

    std::vector<Token> expected{
        {kName, "$", input + 0},
        {kColon, {}, input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelDot) {
    const char *input = ".:";

    std::vector<Token> expected{
        {kName, ".", input + 0},
        {kColon, {}, input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelUnderscore) {
    const char *input = "_:";

    std::vector<Token> expected{
        {kName, "_", input + 0},
        {kColon, {}, input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LabelReserved) {
    const char *input = "define:";

    std::vector<Token> expected{
        {kName, "define", input + 0},
        {kColon, {}, input + 6},
        {kEOF, {}, input + 7},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifier) {
    const char *input = "%AZaz09$._";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "AZaz09$._", input + 1},
        {kEOF, {}, input + 10},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierNumber) {
    const char *input = "%42";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "42", input + 1},
        {kEOF, {}, input + 3},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierNumberHuge) {
    const char *input = "%12345678901234567890";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "12345678901234567890", input + 1},
        {kEOF, {}, input + 21},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierDollar) {
    const char *input = "%$";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "$", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierDot) {
    const char *input = "%.";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, ".", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierUnderscore) {
    const char *input = "%_";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "_", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, LocalIdentifierReserved) {
    const char *input = "%define";

    std::vector<Token> expected{
        {kPercent, {}, input + 0},
        {kName, "define", input + 1},
        {kEOF, {}, input + 7},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifier) {
    const char *input = "@AZaz09$._";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "AZaz09$._", input + 1},
        {kEOF, {}, input + 10},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierNumber) {
    const char *input = "@42";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "42", input + 1},
        {kEOF, {}, input + 3},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierNumberHuge) {
    const char *input = "@12345678901234567890";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "12345678901234567890", input + 1},
        {kEOF, {}, input + 21},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierDollar) {
    const char *input = "@$";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "$", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierDot) {
    const char *input = "@.";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, ".", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierUnderscore) {
    const char *input = "@_";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "_", input + 1},
        {kEOF, {}, input + 2},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, GlobalIdentifierReserved) {
    const char *input = "@define";

    std::vector<Token> expected{
        {kAt, {}, input + 0},
        {kName, "define", input + 1},
        {kEOF, {}, input + 7},
    };

    EXPECT_EQ(lex(input), expected);
}

TEST(LexerTest, String) {
    const char *input = R"(c" ~AZaz\\09\00\1F\7F\FF")";

    std::vector<Token> expected{
        {kString, std::vector<int8_t>{32, 126, 65, 90, 97, 122, 92, 48, 57, 0, 31, 127, -1}, input + 0},
        {kEOF, {}, input + 25},
    };

    EXPECT_EQ(lex(input), expected);
}

} // namespace mini_llvm::ir
