#include <gtest/gtest.h>

#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/ir_parser/Parser.h"

using namespace mini_llvm::ir;

TEST(ir_parser_test, test00) {
    const char *input = R"(
define void @foo() {
0:
    ret void
}
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test01) {
    const char *input = R"(
define internal void @foo() {
0:
    ret void
}
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test02) {
    const char *input = "declare void @foo()";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test03) {
    const char *input = "@foo = global i32 42";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test04) {
    const char *input = "@foo = internal global i32 42";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test05) {
    const char *input = "@foo = external global i32";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test06) {
    const char *input = R"(
@foo = global [2 x [3 x [4 x i32]]] [
    [3 x [4 x i32]] [
        [4 x i32] [i32 42, i32 43, i32 44, i32 45],
        [4 x i32] [i32 46, i32 47, i32 48, i32 49],
        [4 x i32] [i32 50, i32 51, i32 52, i32 53]
    ],
    [3 x [4 x i32]] [
        [4 x i32] [i32 54, i32 55, i32 56, i32 57],
        [4 x i32] [i32 58, i32 59, i32 60, i32 61],
        [4 x i32] [i32 62, i32 63, i32 64, i32 65]
    ]
]
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test07) {
    const char *input = R"(
define i32 @foo() {
0:
    br label %2

1:
    ret i32 %3

2:
    %3 = load i32, ptr @bar
    br label %1
}

@bar = global ptr @baz
@baz = global i32 42
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test08) {
    const char *input = R"(
define void @foo() {
0:
    br label %1

1:
    br label %2

2:
    br label %1
}
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test09) {
    const char *input = R"(
define ptr @foo() {
0:
    ret ptr @bar
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test10) {
    const char *input = R"(
define ptr @foo() {
0:
    ret ptr %1
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test11) {
    const char *input = R"(
define void @foo() {
0:
    br label %1
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test12) {
    const char *input = R"(
@foo = global i32 42

define void @foo() {
0:
    ret void
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test13) {
    const char *input = R"(
define void @foo(i32 %0) {
1:
    %2 = add i32 %0, %0
    %2 = sub i32 %0, %0
    ret void
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test14) {
    const char *input = R"(
define void @foo() {
0:
    ret void

1:
    ret void

1:
    ret void
}
)";

    EXPECT_THROW(parseModule(input), ParseException);
}

TEST(ir_parser_test, test15) {
    const char *input = R"(
define void @foo() {
0:
    call void @bar(i32 42, i32 43, i32 44)
    ret void
}

declare void @bar(i32, i32, i32)
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test16) {
    const char *input = R"(
define void @foo() {
0:
    %1 = call i32 @bar(i32 42, i32 43, i32 44)
    ret void
}

declare i32 @bar(i32, i32, i32)
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test17) {
    const char *input = R"(
define void @foo() {
0:
    %1 = getelementptr [2 x [3 x i32]], ptr @bar, i32 0, i32 0, i32 0
    %2 = load i32, ptr %1
    ret void
}

@bar = global [2 x [3 x i32]] [
    [3 x i32] [i32 42, i32 43, i32 44],
    [3 x i32] [i32 45, i32 46, i32 47]
]
)";

    EXPECT_NO_THROW(parseModule(input));
}

TEST(ir_parser_test, test18) {
    const char *input = R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %3

2:
    br label %3

3:
    %4 = phi i32 [ 42, %1 ], [ 43, %2 ]
    ret void
}
)";

    EXPECT_NO_THROW(parseModule(input));
}
