// SPDX-License-Identifier: MIT

#include <optional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/utils/CommandLineParser.h"
#include "mini-llvm/utils/Expected.h"

namespace mini_llvm {

TEST(CommandLineParserTest, NoArgs) {
    std::vector<std::string> args{"program"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortNoValue) {
    std::vector<std::string> args{"program", "-s"};

    CommandLineParser parser;
    parser.addOption("-s");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortRequiredValue1) {
    std::vector<std::string> args{"program", "-s", "value"};

    CommandLineParser parser;
    parser.addOption("-s:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortRequiredValue2) {
    std::vector<std::string> args{"program", "-svalue"};

    CommandLineParser parser;
    parser.addOption("-s:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortOptionalValue1) {
    std::vector<std::string> args{"program", "-s"};

    CommandLineParser parser;
    parser.addOption("-s::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortOptionalValue2) {
    std::vector<std::string> args{"program", "-s", "value"};

    CommandLineParser parser;
    parser.addOption("-s::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, ShortOptionalValue3) {
    std::vector<std::string> args{"program", "-svalue"};

    CommandLineParser parser;
    parser.addOption("-s::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongNoValue) {
    std::vector<std::string> args{"program", "--long"};

    CommandLineParser parser;
    parser.addOption("--long");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongRequiredValue1) {
    std::vector<std::string> args{"program", "--long", "value"};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongRequiredValue2) {
    std::vector<std::string> args{"program", "--long=value"};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongOptionalValue1) {
    std::vector<std::string> args{"program", "--long"};

    CommandLineParser parser;
    parser.addOption("--long::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongOptionalValue2) {
    std::vector<std::string> args{"program", "--long", "value"};

    CommandLineParser parser;
    parser.addOption("--long::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, LongOptionalValue3) {
    std::vector<std::string> args{"program", "--long=value"};

    CommandLineParser parser;
    parser.addOption("--long::");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "value");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, Bundle) {
    std::vector<std::string> args{"program", "-abcd"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b");
    parser.addOption("-c");
    parser.addOption("-d");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-c");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-d");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, BundleRequiredValue) {
    std::vector<std::string> args{"program", "-abcd"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b:");
    parser.addOption("-c");
    parser.addOption("-d");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), "cd");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, BundleOptionalValue) {
    std::vector<std::string> args{"program", "-abcd"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b::");
    parser.addOption("-c");
    parser.addOption("-d");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), "cd");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, EmptyValueShort) {
    std::vector<std::string> args{"program", "-s", ""};

    CommandLineParser parser;
    parser.addOption("-s:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), "");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, EmptyValueLong1) {
    std::vector<std::string> args{"program", "--long", ""};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, EmptyValueLong2) {
    std::vector<std::string> args{"program", "--long="};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, EmptyValueBundle) {
    std::vector<std::string> args{"program", "-ab", ""};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), "");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, OptionLikeValueShort) {
    std::vector<std::string> args{"program", "-s", "-s"};

    CommandLineParser parser;
    parser.addOption("-s:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), "-s");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, OptionLikeValueLong1) {
    std::vector<std::string> args{"program", "--long", "--long"};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "--long");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, OptionLikeValueLong2) {
    std::vector<std::string> args{"program", "--long=--long"};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), "--long");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, OptionLikeValueBundle) {
    std::vector<std::string> args{"program", "-ab", "-b"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b:");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), "-b");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, MultipleOccurrencesShort) {
    std::vector<std::string> args{"program", "-s", "-s"};

    CommandLineParser parser;
    parser.addOption("-s");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, MultipleOccurrencesLong) {
    std::vector<std::string> args{"program", "--long", "--long"};

    CommandLineParser parser;
    parser.addOption("--long");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, MultipleOccurrencesBundle) {
    std::vector<std::string> args{"program", "-abab"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-a");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-b");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, MissingValueShort) {
    std::vector<std::string> args{"program", "-s"};

    CommandLineParser parser;
    parser.addOption("-s:");

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kMissingValue);
    ASSERT_EQ(result.error().optionName(), "-s");
}

TEST(CommandLineParserTest, MissingValueLong) {
    std::vector<std::string> args{"program", "--long"};

    CommandLineParser parser;
    parser.addOption("--long:");

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kMissingValue);
    ASSERT_EQ(result.error().optionName(), "--long");
}

TEST(CommandLineParserTest, MissingValueBundle) {
    std::vector<std::string> args{"program", "-ab"};

    CommandLineParser parser;
    parser.addOption("-a");
    parser.addOption("-b:");

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kMissingValue);
    ASSERT_EQ(result.error().optionName(), "-b");
}

TEST(CommandLineParserTest, UnexpectedValue) {
    std::vector<std::string> args{"program", "--long=value"};

    CommandLineParser parser;
    parser.addOption("--long");

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kUnexpectedValue);
    ASSERT_EQ(result.error().optionName(), "--long");
}

TEST(CommandLineParserTest, UnrecognizedOptionShort) {
    std::vector<std::string> args{"program", "-s"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kUnrecognizedOption);
    ASSERT_EQ(result.error().optionName(), "-s");
}

TEST(CommandLineParserTest, UnrecognizedOptionLong1) {
    std::vector<std::string> args{"program", "--long"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kUnrecognizedOption);
    ASSERT_EQ(result.error().optionName(), "--long");
}

TEST(CommandLineParserTest, UnrecognizedOptionLong2) {
    std::vector<std::string> args{"program", "--long=value"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kUnrecognizedOption);
    ASSERT_EQ(result.error().optionName(), "--long");
}

TEST(CommandLineParserTest, UnrecognizedOptionBundle) {
    std::vector<std::string> args{"program", "-ab"};

    CommandLineParser parser;
    parser.addOption("-a");

    auto result = parser(args);

    ASSERT_FALSE(result);
    ASSERT_EQ(result.error().kind(), CommandLineParser::ErrorKind::kUnrecognizedOption);
    ASSERT_EQ(result.error().optionName(), "-b");
}

TEST(CommandLineParserTest, Positional) {
    std::vector<std::string> args{"program", "positional"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "positional");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, PositionalEmpty) {
    std::vector<std::string> args{"program", ""};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, PositionalHyphen) {
    std::vector<std::string> args{"program", "-"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "-");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, PositionalDoubleHyphen) {
    std::vector<std::string> args{"program", "--", "--"};

    CommandLineParser parser;

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->separator(), nullptr);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "--");

    ++i;

    ASSERT_EQ(i, j);
}

TEST(CommandLineParserTest, Separator) {
    std::vector<std::string> args{"program", "-s", "--long", "positional", "--", "-s", "--long", "positional"};

    CommandLineParser parser;
    parser.addOption("-s");
    parser.addOption("--long");

    auto result = parser(args);

    ASSERT_TRUE(result);

    auto i = result->begin();
    auto j = result->end();

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "-s");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->option(), nullptr);
    ASSERT_EQ(i->option()->name(), "--long");
    ASSERT_EQ(i->option()->value(), std::nullopt);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "positional");

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->separator(), nullptr);

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "-s");

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "--long");

    ++i;

    ASSERT_NE(i, j);
    ASSERT_NE(i->positional(), nullptr);
    ASSERT_EQ(i->positional()->arg(), "positional");

    ++i;

    ASSERT_EQ(i, j);
}

} // namespace mini_llvm
