// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "mini-llvm/ir_reader/IRReader.h"

namespace mini_llvm::ir {

TEST(IRReaderTest, Function) {
    const char *input = R"(
define void @test() {
0:
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, InternalFunction) {
    const char *input = R"(
define internal void @test() {
0:
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, ExternalFunction) {
    const char *input = "declare void @test()";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, GlobalVar) {
    const char *input = "@test = global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, PrivateGlobalVar) {
    const char *input = "@test = private global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, InternalGlobalVar) {
    const char *input = "@test = internal global i32 42";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, ExternalGlobalVar) {
    const char *input = "@test = external global i32";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, ExternalInternalGlobal) {
    const char *input = "@test = external internal global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ExternalPrivateGlobal) {
    const char *input = "@test = external private global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, InternalPrivateGlobal) {
    const char *input = "@test = internal private global i32 42";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, DeclareInternal) {
    const char *input = "declare internal void @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, DeclarePrivate) {
    const char *input = "declare private void @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, DefineInternalPrivate) {
    const char *input = R"(
define internal private void @test() {
0:
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ReturnTypeLabel) {
    const char *input = "declare label @test()";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ParamTypeVoid) {
    const char *input = "declare void @test(void)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ParamTypeLabel) {
    const char *input = "declare void @test(label)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ElementTypeVoid) {
    const char *input = "@test = external global [2 x [2 x void]]";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ElementTypeLabel) {
    const char *input = "@test = external global [2 x [2 x label]]";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, GlobalVarUseBeforeDeclaration) {
    const char *input = R"(
@test1 = global ptr @test2
@test2 = global i32 42
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, GlobalVarSelfReference) {
    const char *input = "@test = global ptr @test";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, GlobalVarSelfReferenceArrayConstant) {
    const char *input = "@test = global [2 x ptr] [ptr @test, ptr @test]";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, GlobalVarCircularReference) {
    const char *input = R"(
@test1 = global ptr @test2
@test2 = global ptr @test1
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, GlobalVarCircularReferenceArrayConstant) {
    const char *input = R"(
@test1 = global [2 x ptr] [ptr @test2, ptr @test2]
@test2 = global [2 x ptr] [ptr @test1, ptr @test1]
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, LocalIdentifierUseBeforeDeclaration) {
    const char *input = R"(
define i32 @test() {
0:
    br label %2

1:
    ret i32 %3

2:
    %3 = load i32, ptr @test
    br label %1
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, LabelUseBeforeDeclaration) {
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

TEST(IRReaderTest, UndefinedGlobalIdentifier) {
    const char *input = R"(
define ptr @test1() {
0:
    ret ptr @test2
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, UndefinedLocalIdentifier) {
    const char *input = R"(
define ptr @test() {
0:
    ret ptr %1
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, UndefinedLabel) {
    const char *input = R"(
define void @test() {
0:
    br label %1
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, RedefinitionOfGlobalIdentifier) {
    const char *input = R"(
@test = global i32 42

define void @test() {
0:
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, RedefinitionOfLocalIdentifier) {
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

TEST(IRReaderTest, RedefinitionOfLabel) {
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

TEST(IRReaderTest, EmptyFunctionBody) {
    const char *input = "define void @test() {}";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, InvalidInstructionMnemonic) {
    const char *input = R"(
define void @test() {
0:
    invalid
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, NamedVoid) {
    const char *input = R"(
define void @test() {
0:
    %1 = ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, UnnamedNonVoid) {
    const char *input = R"(
define void @test() {
0:
    add i32 0, 0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, CallVoid) {
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

TEST(IRReaderTest, CallNonVoid) {
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

TEST(IRReaderTest, GetElementPtr) {
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

TEST(IRReaderTest, GetElementPtrTooManyIndices) {
    const char *input = R"(
define ptr @test(ptr %0) {
1:
  %2 = getelementptr [2 x [3 x i32]], ptr %0, i32 0, i32 0, i32 0, i32 0
  ret ptr %2
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, Phi) {
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

TEST(IRReaderTest, SelfReferenceAdd) {
    const char *input = R"(
define void @test(ptr %0) {
1:
    %2 = add i32 %2, 1
    store i32 %2, ptr %0
    ret void
}
)";

    parseModule(input);
}

TEST(IRReaderTest, SelfReferenceSExt) {
    const char *input = R"(
define void @test(ptr %0) {
1:
    %2 = sext i32 %2 to i64
    store i64 %2, ptr %0
    ret void
}
)";

    parseModule(input);
}

TEST(IRReaderTest, CircularReference1) {
    const char *input = R"(
define void @test(ptr %0) {
1:
    %2 = add i32 %3, 1
    %3 = add i32 %2, 1
    store i32 %3, ptr %0
    ret void
}
)";

    parseModule(input);
}

TEST(IRReaderTest, CircularReference2) {
    const char *input = R"(
define void @test(ptr %0) {
1:
    %2 = add i32 %3, 1
    %3 = add i32 %4, 1
    %4 = add i32 %2, 1
    store i32 %4, ptr %0
    ret void
}
)";

    parseModule(input);
}

TEST(IRReaderTest, AddDouble) {
    const char *input = R"(
define void @test() {
0:
    %1 = add double 0x0000000000000000, 0x0000000000000000
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, ICmpDouble) {
    const char *input = R"(
define void @test() {
0:
    %1 = icmp eq double 0x0000000000000000, 0x0000000000000000
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, TruncDoubleToI32) {
    const char *input = R"(
define void @test() {
0:
    %1 = trunc double 0x0000000000000000 to i32
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, TruncI64ToFloat) {
    const char *input = R"(
define void @test() {
0:
    %1 = trunc i64 0 to float
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, FNegI32) {
    const char *input = R"(
define void @test() {
0:
    %1 = fneg i32 0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, FAddI32) {
    const char *input = R"(
define void @test() {
0:
    %1 = fadd i32 0, 0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, FCmpI32) {
    const char *input = R"(
define void @test() {
0:
    %1 = fcmp oeq i32 0, 0
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, FPTruncI64ToFloat) {
    const char *input = R"(
define void @test() {
0:
    %1 = fptrunc i64 0 to float
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, FPTruncDoubleToI32) {
    const char *input = R"(
define void @test() {
0:
    %1 = fptrunc double 0x0000000000000000 to i32
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, BitCastPtrToNonPtr) {
    const char *input = R"(
define void @test() {
0:
    %1 = bitcast ptr null to i64
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, BitCastNonPtrToPtr) {
    const char *input = R"(
define void @test() {
0:
    %1 = bitcast i64 0 to ptr
    ret void
}
)";

    EXPECT_FALSE(parseModule(input));
}

TEST(IRReaderTest, BitCastPtrToPtr) {
    const char *input = R"(
define void @test() {
0:
    %1 = bitcast ptr null to ptr
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, ZeroInitializerGlobalVar) {
    const char *input = "@test = global i32 zeroinitializer";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, ZeroInitializerOperand) {
    const char *input = R"(
define void @test() {
0:
    %1 = add i32 0, zeroinitializer
    ret void
}
)";

    EXPECT_TRUE(parseModule(input));
}

TEST(IRReaderTest, NegativeNumElements) {
    const char *input = "@test = global [-1 x i32] zeroinitializer";

    EXPECT_FALSE(parseModule(input));
}

} // namespace mini_llvm::ir
