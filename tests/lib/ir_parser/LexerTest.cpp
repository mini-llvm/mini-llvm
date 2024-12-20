#include <cstdint>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir_parser/Lexer.h"
#include "mini-llvm/ir_parser/Token.h"

using namespace mini_llvm::ir;

using enum Token::Kind;

TEST(LexerTest, test00) {
    const char *input = "";

    std::vector<Token> expectedOutput{
        {kEOF, {}, input},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test01) {
    const char *input = R"(
; Comment
@foo = ; Comment
; Comment
; Comment
global i32 42 ; Comment
; Comment
)";

    std::vector<Token> expectedOutput{
        {kAt,     {},    input + 11},
        {kName,   "foo", input + 12},
        {kEqual,  {},    input + 16},
        {kGlobal, {},    input + 48},
        {kI32,    {},    input + 55},
        {kNumber, 42,    input + 59},
        {kEOF,    {},    input + 82},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test02) {
    const char *input = "9223372036854775807";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::max(), input + 0},
        {kEOF, {}, input + 19},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test03) {
    const char *input = "-9223372036854775808";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::min(), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test04) {
    const char *input = "18446744073709551615";

    std::vector<Token> expectedOutput{
        {kNumber, static_cast<int64_t>(std::numeric_limits<uint64_t>::max()), input + 0},
        {kEOF, {}, input + 20},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test05) {
    const char *input = "0x7fffffffffffffff";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::max(), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test06) {
    const char *input = "0x8000000000000000";

    std::vector<Token> expectedOutput{
        {kNumber, std::numeric_limits<int64_t>::min(), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test07) {
    const char *input = "0xffffffffffffffff";

    std::vector<Token> expectedOutput{
        {kNumber, static_cast<int64_t>(std::numeric_limits<uint64_t>::max()), input + 0},
        {kEOF, {}, input + 18},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test08) {
    const char *input = "_foo.42:";

    std::vector<Token> expectedOutput{
        {kName,  "_foo.42", input + 0},
        {kColon, {},        input + 7},
        {kEOF,   {},        input + 8},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test09) {
    const char *input = "define:";

    std::vector<Token> expectedOutput{
        {kName, "define", input + 0},
        {kColon, {},      input + 6},
        {kEOF,   {},      input + 7},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test10) {
    const char *input = "42:";

    std::vector<Token> expectedOutput{
        {kName, "42", input + 0},
        {kColon, {},  input + 2},
        {kEOF,   {},  input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test11) {
    const char *input = "%_foo.42";

    std::vector<Token> expectedOutput{
        {kPercent, {},        input + 0},
        {kName,    "_foo.42", input + 1},
        {kEOF,     {},        input + 8},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test12) {
    const char *input = "@define";

    std::vector<Token> expectedOutput{
        {kAt,   {},       input + 0},
        {kName, "define", input + 1},
        {kEOF,  {},       input + 7},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test13) {
    const char *input = "%42";

    std::vector<Token> expectedOutput{
        {kPercent, {},   input + 0},
        {kName,    "42", input + 1},
        {kEOF,     {},   input + 3},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}

TEST(LexerTest, test14) {
    const char *input = R"(c"foo\\42\0A\00")";

    std::vector<Token> expectedOutput{
        {kString, std::vector<int8_t>{0x66, 0x6f, 0x6f, 0x5c, 0x34, 0x32, 0x0a, 0x00}, input + 0},
        {kEOF, {}, input + 16},
    };

    EXPECT_EQ(lex(input), expectedOutput);
}
