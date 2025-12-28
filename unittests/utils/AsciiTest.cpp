// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "mini-llvm/utils/Ascii.h"

namespace mini_llvm {

TEST(AsciiTest, isAsciiPrintable) {
    EXPECT_TRUE(isAsciiPrintable(' '));
    EXPECT_TRUE(isAsciiPrintable('~'));
    EXPECT_TRUE(isAsciiPrintable('0'));
    EXPECT_TRUE(isAsciiPrintable('9'));
    EXPECT_TRUE(isAsciiPrintable('A'));
    EXPECT_TRUE(isAsciiPrintable('Z'));
    EXPECT_TRUE(isAsciiPrintable('a'));
    EXPECT_TRUE(isAsciiPrintable('z'));

    EXPECT_FALSE(isAsciiPrintable('\x00'));
    EXPECT_FALSE(isAsciiPrintable('\x1f'));
    EXPECT_FALSE(isAsciiPrintable('\x80'));
    EXPECT_FALSE(isAsciiPrintable('\xff'));
}

TEST(AsciiTest, isAsciiUpper) {
    EXPECT_TRUE(isAsciiUpper('A'));
    EXPECT_TRUE(isAsciiUpper('Z'));

    EXPECT_FALSE(isAsciiUpper('a'));
    EXPECT_FALSE(isAsciiUpper('z'));
    EXPECT_FALSE(isAsciiUpper('0'));
    EXPECT_FALSE(isAsciiUpper('9'));
    EXPECT_FALSE(isAsciiUpper(' '));
    EXPECT_FALSE(isAsciiUpper('~'));
    EXPECT_FALSE(isAsciiUpper('\x00'));
    EXPECT_FALSE(isAsciiUpper('\x1f'));
    EXPECT_FALSE(isAsciiUpper('\x80'));
    EXPECT_FALSE(isAsciiUpper('\xff'));
}

TEST(AsciiTest, isAsciiLower) {
    EXPECT_TRUE(isAsciiLower('a'));
    EXPECT_TRUE(isAsciiLower('z'));

    EXPECT_FALSE(isAsciiLower('A'));
    EXPECT_FALSE(isAsciiLower('Z'));
    EXPECT_FALSE(isAsciiLower('0'));
    EXPECT_FALSE(isAsciiLower('9'));
    EXPECT_FALSE(isAsciiLower(' '));
    EXPECT_FALSE(isAsciiLower('~'));
    EXPECT_FALSE(isAsciiLower('\x00'));
    EXPECT_FALSE(isAsciiLower('\x1f'));
    EXPECT_FALSE(isAsciiLower('\x80'));
    EXPECT_FALSE(isAsciiLower('\xff'));
}

TEST(AsciiTest, isAsciiLetter) {
    EXPECT_TRUE(isAsciiLetter('A'));
    EXPECT_TRUE(isAsciiLetter('Z'));
    EXPECT_TRUE(isAsciiLetter('a'));
    EXPECT_TRUE(isAsciiLetter('z'));

    EXPECT_FALSE(isAsciiLetter('0'));
    EXPECT_FALSE(isAsciiLetter('9'));
    EXPECT_FALSE(isAsciiLetter(' '));
    EXPECT_FALSE(isAsciiLetter('~'));
    EXPECT_FALSE(isAsciiLetter('\x00'));
    EXPECT_FALSE(isAsciiLetter('\x1f'));
    EXPECT_FALSE(isAsciiLetter('\x80'));
    EXPECT_FALSE(isAsciiLetter('\xff'));
}

TEST(AsciiTest, isAsciiDigit) {
    EXPECT_TRUE(isAsciiDigit('0'));
    EXPECT_TRUE(isAsciiDigit('9'));

    EXPECT_FALSE(isAsciiDigit('A'));
    EXPECT_FALSE(isAsciiDigit('Z'));
    EXPECT_FALSE(isAsciiDigit('a'));
    EXPECT_FALSE(isAsciiDigit('z'));
    EXPECT_FALSE(isAsciiDigit(' '));
    EXPECT_FALSE(isAsciiDigit('~'));
    EXPECT_FALSE(isAsciiDigit('\x00'));
    EXPECT_FALSE(isAsciiDigit('\x1f'));
    EXPECT_FALSE(isAsciiDigit('\x80'));
    EXPECT_FALSE(isAsciiDigit('\xff'));
}

TEST(AsciiTest, isAsciiLetterOrDigit) {
    EXPECT_TRUE(isAsciiLetterOrDigit('A'));
    EXPECT_TRUE(isAsciiLetterOrDigit('Z'));
    EXPECT_TRUE(isAsciiLetterOrDigit('a'));
    EXPECT_TRUE(isAsciiLetterOrDigit('z'));
    EXPECT_TRUE(isAsciiLetterOrDigit('0'));
    EXPECT_TRUE(isAsciiLetterOrDigit('9'));

    EXPECT_FALSE(isAsciiLetterOrDigit(' '));
    EXPECT_FALSE(isAsciiLetterOrDigit('~'));
    EXPECT_FALSE(isAsciiLetterOrDigit('\x00'));
    EXPECT_FALSE(isAsciiLetterOrDigit('\x1f'));
    EXPECT_FALSE(isAsciiLetterOrDigit('\x80'));
    EXPECT_FALSE(isAsciiLetterOrDigit('\xff'));
}

TEST(AsciiTest, isAsciiHexDigit) {
    EXPECT_TRUE(isAsciiHexDigit('0'));
    EXPECT_TRUE(isAsciiHexDigit('9'));
    EXPECT_TRUE(isAsciiHexDigit('A'));
    EXPECT_TRUE(isAsciiHexDigit('F'));
    EXPECT_TRUE(isAsciiHexDigit('a'));
    EXPECT_TRUE(isAsciiHexDigit('f'));

    EXPECT_FALSE(isAsciiHexDigit('G'));
    EXPECT_FALSE(isAsciiHexDigit('Z'));
    EXPECT_FALSE(isAsciiHexDigit('g'));
    EXPECT_FALSE(isAsciiHexDigit('z'));
    EXPECT_FALSE(isAsciiHexDigit(' '));
    EXPECT_FALSE(isAsciiHexDigit('~'));
    EXPECT_FALSE(isAsciiHexDigit('\x00'));
    EXPECT_FALSE(isAsciiHexDigit('\x1f'));
    EXPECT_FALSE(isAsciiHexDigit('\x80'));
    EXPECT_FALSE(isAsciiHexDigit('\xff'));
}

TEST(AsciiTest, toAsciiLower) {
    EXPECT_EQ(toAsciiLower('A'), 'a');
    EXPECT_EQ(toAsciiLower('Z'), 'z');

    EXPECT_EQ(toAsciiLower('a'), 'a');
    EXPECT_EQ(toAsciiLower('z'), 'z');
    EXPECT_EQ(toAsciiLower('0'), '0');
    EXPECT_EQ(toAsciiLower('9'), '9');
    EXPECT_EQ(toAsciiLower(' '), ' ');
    EXPECT_EQ(toAsciiLower('~'), '~');
    EXPECT_EQ(toAsciiLower('\x00'), '\x00');
    EXPECT_EQ(toAsciiLower('\x1f'), '\x1f');
    EXPECT_EQ(toAsciiLower('\x80'), '\x80');
    EXPECT_EQ(toAsciiLower('\xff'), '\xff');
}

TEST(AsciiTest, toAsciiUpper) {
    EXPECT_EQ(toAsciiUpper('a'), 'A');
    EXPECT_EQ(toAsciiUpper('z'), 'Z');

    EXPECT_EQ(toAsciiUpper('A'), 'A');
    EXPECT_EQ(toAsciiUpper('Z'), 'Z');
    EXPECT_EQ(toAsciiUpper('0'), '0');
    EXPECT_EQ(toAsciiUpper('9'), '9');
    EXPECT_EQ(toAsciiUpper(' '), ' ');
    EXPECT_EQ(toAsciiUpper('~'), '~');
    EXPECT_EQ(toAsciiUpper('\x00'), '\x00');
    EXPECT_EQ(toAsciiUpper('\x1f'), '\x1f');
    EXPECT_EQ(toAsciiUpper('\x80'), '\x80');
    EXPECT_EQ(toAsciiUpper('\xff'), '\xff');
}

} // namespace mini_llvm
