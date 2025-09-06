// SPDX-License-Identifier: MIT

#include <format>

#include <gtest/gtest.h>

#include "mini-llvm/utils/StringJoiner.h"

namespace mini_llvm {

TEST(StringJoinerTest, PrefixSuffix) {
    StringJoiner joiner(", ", "[", "]");

    EXPECT_EQ(joiner.toString(), "[]");

    joiner.add("element1");
    EXPECT_EQ(joiner.toString(), "[element1]");

    joiner.add("element2");
    EXPECT_EQ(joiner.toString(), "[element1, element2]");

    joiner.add("element3");
    EXPECT_EQ(joiner.toString(), "[element1, element2, element3]");
}

TEST(StringJoinerTest, NoPrefixSuffix) {
    StringJoiner joiner(", ");

    EXPECT_EQ(joiner.toString(), "");

    joiner.add("element1");
    EXPECT_EQ(joiner.toString(), "element1");

    joiner.add("element2");
    EXPECT_EQ(joiner.toString(), "element1, element2");

    joiner.add("element3");
    EXPECT_EQ(joiner.toString(), "element1, element2, element3");
}

TEST(StringJoinerTest, EmptyPrefixSuffix) {
    StringJoiner joiner(", ", "", "");

    EXPECT_EQ(joiner.toString(), "");

    joiner.add("element1");
    EXPECT_EQ(joiner.toString(), "element1");

    joiner.add("element2");
    EXPECT_EQ(joiner.toString(), "element1, element2");

    joiner.add("element3");
    EXPECT_EQ(joiner.toString(), "element1, element2, element3");
}

TEST(StringJoinerTest, EmptyElement) {
    StringJoiner joiner(", ", "[", "]");

    joiner.add("");
    EXPECT_EQ(joiner.toString(), "[]");

    joiner.add("");
    EXPECT_EQ(joiner.toString(), "[, ]");

    joiner.add("");
    EXPECT_EQ(joiner.toString(), "[, , ]");
}

TEST(StringJoinerTest, EmptyDelimiter) {
    StringJoiner joiner("", "[", "]");

    joiner.add("element1");
    joiner.add("element2");
    joiner.add("element3");

    EXPECT_EQ(joiner.toString(), "[element1element2element3]");
}

TEST(StringJoinerTest, Chained) {
    StringJoiner joiner(", ", "[", "]");

    joiner.add("element1").add("element2").add("element3");

    EXPECT_EQ(joiner.toString(), "[element1, element2, element3]");
}

TEST(StringJoinerTest, Format) {
    StringJoiner joiner(", ", "[", "]");

    joiner.add("{}", 1);

    EXPECT_EQ(joiner.toString(), "[1]");
}

TEST(StringJoinerTest, ChainedFormat) {
    StringJoiner joiner(", ", "[", "]");

    joiner.add("{}", 1).add("{}", 2).add("{}", 3);

    EXPECT_EQ(joiner.toString(), "[1, 2, 3]");
}

TEST(StringJoinerTest, StdFormat) {
    StringJoiner joiner(", ", "[", "]");

    joiner.add("element1");
    joiner.add("element2");
    joiner.add("element3");

    EXPECT_EQ(std::format("{}", joiner), "[element1, element2, element3]");
}

} // namespace mini_llvm
