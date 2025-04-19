#include <gtest/gtest.h>

#include "mini-llvm/ir_reader/IRReader.h"

using namespace mini_llvm::ir;

TEST(IRReaderTest, function) {
    const char *input = R"(
define void @test() {
0:
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, internalFunction) {
    const char *input = R"(
define internal void @test() {
0:
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, externalFunction) {
    const char *input = "declare void @test()";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, globalVar) {
    const char *input = "@test = global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, privateGlobalVar) {
    const char *input = "@test = private global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, internalGlobalVar) {
    const char *input = "@test = internal global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, externalGlobalVar) {
    const char *input = "@test = external global i32";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, externalInternalGlobal) {
    const char *input = "@test = external internal global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, externalPrivateGlobal) {
    const char *input = "@test = external private global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, internalPrivateGlobal) {
    const char *input = "@test = internal private global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, declareInternal) {
    const char *input = "declare internal void @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, declarePrivate) {
    const char *input = "declare private void @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, defineInternalPrivate) {
    const char *input = R"(
define internal private void @test() {
0:
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, returnTypeLabel) {
    const char *input = "declare label @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, globalVarUseBeforeDeclaration) {
    const char *input = R"(
define i32 @test1() {
0:
    br label %2

1:
    ret i32 %3

2:
    %3 = load i32, ptr @test2
    br label %1
}

@test2 = global ptr @test3
@test3 = global i32 42
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, labelUseBeforeDeclaration) {
    const char *input = R"(
define void @test() {
0:
    br label %1

1:
    br label %2

2:
    br label %1
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, undefinedGlobalIdentifier) {
    const char *input = R"(
define ptr @test1() {
0:
    ret ptr @test2
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, undefinedLocalIdentifier) {
    const char *input = R"(
define ptr @test() {
0:
    ret ptr %1
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, undefinedLabel) {
    const char *input = R"(
define void @test() {
0:
    br label %1
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, redefinitionOfGlobalIdentifier) {
    const char *input = R"(
@test = global i32 42

define void @test() {
0:
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, redefinitionOfLocalIdentifier) {
    const char *input = R"(
define void @test(i32 %0) {
1:
    %2 = add i32 %0, %0
    %2 = sub i32 %0, %0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, redefinitionOfLabel) {
    const char *input = R"(
define void @test() {
0:
    ret void

1:
    ret void

1:
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, emptyFunctionBody) {
    const char *input = "define void @test() {}";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, invalidInstructionMnemonic) {
    const char *input = R"(
define void @test() {
0:
    invalid
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, namedVoid) {
    const char *input = R"(
define void @test() {
0:
    %1 = ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, unnamedNonVoid) {
    const char *input = R"(
define void @test() {
0:
    add i32 0, 0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, callVoid) {
    const char *input = R"(
define void @test1() {
0:
    call void @test2(i32 42, i32 43, i32 44)
    ret void
}

declare void @test2(i32, i32, i32)
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, callNonVoid) {
    const char *input = R"(
define void @test1() {
0:
    %1 = call i32 @test2(i32 42, i32 43, i32 44)
    ret void
}

declare i32 @test2(i32, i32, i32)
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, getElementPtr) {
    const char *input = R"(
define void @test1() {
0:
    %1 = getelementptr [2 x [3 x i32]], ptr @test2, i32 0, i32 0, i32 0
    %2 = load i32, ptr %1
    ret void
}

@test2 = global [2 x [3 x i32]] [
    [3 x i32] [i32 42, i32 43, i32 44],
    [3 x i32] [i32 45, i32 46, i32 47]
]
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, phi) {
    const char *input = R"(
define void @test(i1 %0) {
1:
    br i1 %0, label %2, label %3

2:
    br label %3

3:
    %4 = phi i32 [ 42, %1 ], [ 43, %2 ]
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, selfReferentialAdd) {
    const char *input = R"(
define void @test() {
0:
    %1 = add i32 %1, %1
    ret void
}
)";

    parseModule(input);
}

TEST(IRReaderTest, selfReferentialSExt) {
    const char *input = R"(
define void @test() {
0:
    %1 = sext i32 %1 to i64
    ret void
}
)";

    parseModule(input);
}
