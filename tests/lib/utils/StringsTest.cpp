#include <string>

#include <gtest/gtest.h>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;

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
