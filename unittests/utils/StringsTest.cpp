// SPDX-License-Identifier: MIT

#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include "mini-llvm/utils/Strings.h"

namespace mini_llvm {

TEST(StringsTest, ToStringPositive) {
    EXPECT_EQ(toString(INT32_C(12345), 10), "12345");
    EXPECT_EQ(toString(INT32_C(12345), 2), "11000000111001");
    EXPECT_EQ(toString(INT32_C(12345), 8), "30071");
    EXPECT_EQ(toString(INT32_C(12345), 16), "3039");
    EXPECT_EQ(toString(INT32_C(12345), 36), "9ix");
    EXPECT_EQ(toString(INT32_C(12345), 62), "3d7");

    EXPECT_EQ(toString(INT64_C(12345), 10), "12345");
    EXPECT_EQ(toString(INT64_C(12345), 2), "11000000111001");
    EXPECT_EQ(toString(INT64_C(12345), 8), "30071");
    EXPECT_EQ(toString(INT64_C(12345), 16), "3039");
    EXPECT_EQ(toString(INT64_C(12345), 36), "9ix");
    EXPECT_EQ(toString(INT64_C(12345), 62), "3d7");

    EXPECT_EQ(toString(UINT32_C(12345), 10), "12345");
    EXPECT_EQ(toString(UINT32_C(12345), 2), "11000000111001");
    EXPECT_EQ(toString(UINT32_C(12345), 8), "30071");
    EXPECT_EQ(toString(UINT32_C(12345), 16), "3039");
    EXPECT_EQ(toString(UINT32_C(12345), 36), "9ix");
    EXPECT_EQ(toString(UINT32_C(12345), 62), "3d7");

    EXPECT_EQ(toString(UINT64_C(12345), 10), "12345");
    EXPECT_EQ(toString(UINT64_C(12345), 2), "11000000111001");
    EXPECT_EQ(toString(UINT64_C(12345), 8), "30071");
    EXPECT_EQ(toString(UINT64_C(12345), 16), "3039");
    EXPECT_EQ(toString(UINT64_C(12345), 36), "9ix");
    EXPECT_EQ(toString(UINT64_C(12345), 62), "3d7");
}

TEST(StringsTest, ToStringNegative) {
    EXPECT_EQ(toString(INT32_C(-12345), 10), "-12345");
    EXPECT_EQ(toString(INT32_C(-12345), 2), "-11000000111001");
    EXPECT_EQ(toString(INT32_C(-12345), 8), "-30071");
    EXPECT_EQ(toString(INT32_C(-12345), 16), "-3039");
    EXPECT_EQ(toString(INT32_C(-12345), 36), "-9ix");
    EXPECT_EQ(toString(INT32_C(-12345), 62), "-3d7");

    EXPECT_EQ(toString(INT64_C(-12345), 10), "-12345");
    EXPECT_EQ(toString(INT64_C(-12345), 2), "-11000000111001");
    EXPECT_EQ(toString(INT64_C(-12345), 8), "-30071");
    EXPECT_EQ(toString(INT64_C(-12345), 16), "-3039");
    EXPECT_EQ(toString(INT64_C(-12345), 36), "-9ix");
    EXPECT_EQ(toString(INT64_C(-12345), 62), "-3d7");
}

TEST(StringsTest, ToStringZero) {
    EXPECT_EQ(toString(INT32_C(0), 10), "0");
    EXPECT_EQ(toString(INT32_C(0), 2), "0");
    EXPECT_EQ(toString(INT32_C(0), 8), "0");
    EXPECT_EQ(toString(INT32_C(0), 16), "0");
    EXPECT_EQ(toString(INT32_C(0), 36), "0");
    EXPECT_EQ(toString(INT32_C(0), 62), "0");

    EXPECT_EQ(toString(INT64_C(0), 10), "0");
    EXPECT_EQ(toString(INT64_C(0), 2), "0");
    EXPECT_EQ(toString(INT64_C(0), 8), "0");
    EXPECT_EQ(toString(INT64_C(0), 16), "0");
    EXPECT_EQ(toString(INT64_C(0), 36), "0");
    EXPECT_EQ(toString(INT64_C(0), 62), "0");

    EXPECT_EQ(toString(UINT32_C(0), 10), "0");
    EXPECT_EQ(toString(UINT32_C(0), 2), "0");
    EXPECT_EQ(toString(UINT32_C(0), 8), "0");
    EXPECT_EQ(toString(UINT32_C(0), 16), "0");
    EXPECT_EQ(toString(UINT32_C(0), 36), "0");
    EXPECT_EQ(toString(UINT32_C(0), 62), "0");

    EXPECT_EQ(toString(UINT64_C(0), 10), "0");
    EXPECT_EQ(toString(UINT64_C(0), 2), "0");
    EXPECT_EQ(toString(UINT64_C(0), 8), "0");
    EXPECT_EQ(toString(UINT64_C(0), 16), "0");
    EXPECT_EQ(toString(UINT64_C(0), 36), "0");
    EXPECT_EQ(toString(UINT64_C(0), 62), "0");
}

TEST(StringsTest, ToStringMax) {
    EXPECT_EQ(toString(INT32_MAX, 10), "2147483647");
    EXPECT_EQ(toString(INT32_MAX, 2), "1111111111111111111111111111111");
    EXPECT_EQ(toString(INT32_MAX, 8), "17777777777");
    EXPECT_EQ(toString(INT32_MAX, 16), "7fffffff");
    EXPECT_EQ(toString(INT32_MAX, 36), "zik0zj");
    EXPECT_EQ(toString(INT32_MAX, 62), "2lkCB1");

    EXPECT_EQ(toString(INT64_MAX, 10), "9223372036854775807");
    EXPECT_EQ(toString(INT64_MAX, 2), "111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(toString(INT64_MAX, 8), "777777777777777777777");
    EXPECT_EQ(toString(INT64_MAX, 16), "7fffffffffffffff");
    EXPECT_EQ(toString(INT64_MAX, 36), "1y2p0ij32e8e7");
    EXPECT_EQ(toString(INT64_MAX, 62), "aZl8N0y58M7");

    EXPECT_EQ(toString(UINT32_MAX, 10), "4294967295");
    EXPECT_EQ(toString(UINT32_MAX, 2), "11111111111111111111111111111111");
    EXPECT_EQ(toString(UINT32_MAX, 8), "37777777777");
    EXPECT_EQ(toString(UINT32_MAX, 16), "ffffffff");
    EXPECT_EQ(toString(UINT32_MAX, 36), "1z141z3");
    EXPECT_EQ(toString(UINT32_MAX, 62), "4GFfc3");

    EXPECT_EQ(toString(UINT64_MAX, 10), "18446744073709551615");
    EXPECT_EQ(toString(UINT64_MAX, 2), "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(toString(UINT64_MAX, 8), "1777777777777777777777");
    EXPECT_EQ(toString(UINT64_MAX, 16), "ffffffffffffffff");
    EXPECT_EQ(toString(UINT64_MAX, 36), "3w5e11264sgsf");
    EXPECT_EQ(toString(UINT64_MAX, 62), "lYGhA16ahyf");
}

TEST(StringsTest, ToStringMin) {
    EXPECT_EQ(toString(INT32_MIN, 10), "-2147483648");
    EXPECT_EQ(toString(INT32_MIN, 2), "-10000000000000000000000000000000");
    EXPECT_EQ(toString(INT32_MIN, 8), "-20000000000");
    EXPECT_EQ(toString(INT32_MIN, 16), "-80000000");
    EXPECT_EQ(toString(INT32_MIN, 36), "-zik0zk");
    EXPECT_EQ(toString(INT32_MIN, 62), "-2lkCB2");

    EXPECT_EQ(toString(INT64_MIN, 10), "-9223372036854775808");
    EXPECT_EQ(toString(INT64_MIN, 2), "-1000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(toString(INT64_MIN, 8), "-1000000000000000000000");
    EXPECT_EQ(toString(INT64_MIN, 16), "-8000000000000000");
    EXPECT_EQ(toString(INT64_MIN, 36), "-1y2p0ij32e8e8");
    EXPECT_EQ(toString(INT64_MIN, 62), "-aZl8N0y58M8");
}

TEST(StringsTest, NormalizeLineEndingsEmpty) {
    std::string str;
    normalizeLineEndings(str);
    EXPECT_EQ(str, "");
}

TEST(StringsTest, NormalizeLineEndingsUnixStyle) {
    std::string str = "Line 1\nLine 2\nLine 3\n";
    normalizeLineEndings(str);
    EXPECT_EQ(str, "Line 1\nLine 2\nLine 3\n");
}

TEST(StringsTest, NormalizeLineEndingsWindowsStyle) {
    std::string str = "Line 1\r\nLine 2\r\nLine 3\r\n";
    normalizeLineEndings(str);
    EXPECT_EQ(str, "Line 1\nLine 2\nLine 3\n");
}

TEST(StringsTest, NormalizeLineEndingsMacStyle) {
    std::string str = "Line 1\rLine 2\rLine 3\r";
    normalizeLineEndings(str);
    EXPECT_EQ(str, "Line 1\nLine 2\nLine 3\n");
}

TEST(StringsTest, NormalizeLineEndingsMixedStyles) {
    std::string str = "Line 1\nLine 2\r\nLine 3\rLine 4\n";
    normalizeLineEndings(str);
    EXPECT_EQ(str, "Line 1\nLine 2\nLine 3\nLine 4\n");
}

} // namespace mini_llvm
