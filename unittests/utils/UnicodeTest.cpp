// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "mini-llvm/utils/Unicode.h"

namespace mini_llvm {

TEST(UnicodeTest, DecodeUtf8OneByte) {
    const char *str = "A"; // U+0041
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x41);
    EXPECT_EQ(ptr - str, 1);
}

TEST(UnicodeTest, DecodeUtf8OneByteMin) {
    const char *str = "\x00"; // U+0000
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x00);
    EXPECT_EQ(ptr - str, 1);
}

TEST(UnicodeTest, DecodeUtf8OneByteMax) {
    const char *str = "\x7f"; // U+007F
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x7f);
    EXPECT_EQ(ptr - str, 1);
}

TEST(UnicodeTest, DecodeUtf8TwoBytes) {
    const char *str = "©"; // U+00A9
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0xa9);
    EXPECT_EQ(ptr - str, 2);
}

TEST(UnicodeTest, DecodeUtf8TwoBytesMin) {
    const char *str = "\xc2\x80"; // U+0080
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x80);
    EXPECT_EQ(ptr - str, 2);
}

TEST(UnicodeTest, DecodeUtf8TwoBytesMax) {
    const char *str = "\xdf\xbf"; // U+07FF
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x7ff);
    EXPECT_EQ(ptr - str, 2);
}

TEST(UnicodeTest, DecodeUtf8ThreeBytes) {
    const char *str = "中"; // U+4E2D
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x4e2d);
    EXPECT_EQ(ptr - str, 3);
}

TEST(UnicodeTest, DecodeUtf8ThreeBytesMin) {
    const char *str = "\xe0\xa0\x80"; // U+0800
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x800);
    EXPECT_EQ(ptr - str, 3);
}

TEST(UnicodeTest, DecodeUtf8ThreeBytesMax) {
    const char *str = "\xef\xbf\xbf"; // U+FFFF
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0xffff);
    EXPECT_EQ(ptr - str, 3);
}

TEST(UnicodeTest, DecodeUtf8FourBytes) {
    const char *str = "🎈"; // U+1F388
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x1f388);
    EXPECT_EQ(ptr - str, 4);
}

TEST(UnicodeTest, DecodeUtf8FourBytesMin) {
    const char *str = "\xf0\x90\x80\x80"; // U+10000
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x10000);
    EXPECT_EQ(ptr - str, 4);
}

TEST(UnicodeTest, DecodeUtf8FourBytesMax) {
    const char *str = "\xf4\x8f\xbf\xbf"; // U+10FFFF
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, 0x10ffff);
    EXPECT_EQ(ptr - str, 4);
}

TEST(UnicodeTest, DecodeUtf8IncompleteTwoBytes) {
    const char *str = "\xc2";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8IncompleteThreeBytes) {
    const char *str = "\xe4\xb8";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8IncompleteFourBytes) {
    const char *str = "\xf0\x9f\x8e";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8OverlongTwoBytes) {
    const char *str = "\xc0\xc1";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8OverlongThreeBytes) {
    const char *str = "\xe0\x82\xa9";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8OverlongFourBytes) {
    const char *str = "\xf0\x84\xb8\xad";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidStartByte1) {
    const char *str = "\x80";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidStartByte2) {
    const char *str = "\xf8";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteTwoBytes) {
    const char *str = "\xc2\x40";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteThreeBytes1) {
    const char *str = "\xe4\x40\xad";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteThreeBytes2) {
    const char *str = "\xe4\xb8\x40";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteFourBytes1) {
    const char *str = "\xf0\x40\x8e\x88";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteFourBytes2) {
    const char *str = "\xf0\x9f\x40\x88";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8InvalidContinuationByteFourBytes3) {
    const char *str = "\xf0\x9f\x8e\x40";
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateHigh) {
    const char *str = "\xed\xa8\x80"; // U+DA00
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateHighMin) {
    const char *str = "\xed\xa0\x80"; // U+D800
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateHighMax) {
    const char *str = "\xed\xaf\xbf"; // U+DBFF
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateLow) {
    const char *str = "\xed\b8\x80"; // U+DE00
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateLowMin) {
    const char *str = "\xed\xb0\x80"; // U+DC00
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8SurrogateLowMax) {
    const char *str = "\xed\xbf\xbf"; // U+DFFF
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

TEST(UnicodeTest, DecodeUtf8OutOfRange) {
    const char *str = "\xf5\x80\x80\x80"; // U+110000
    const char *ptr = str;
    char32_t cp = decodeUtf8(ptr);
    EXPECT_EQ(cp, static_cast<char32_t>(-1));
}

} // namespace mini_llvm
