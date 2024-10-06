#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/opt/ir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Not;

using namespace mini_llvm::ir;

TEST(DeadCodeEliminationTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void
}
)");

    EXPECT_FALSE(DeadCodeElimination().runOnFunction(*F));
}

TEST(DeadCodeEliminationTest, test1) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    %1 = add i32 42, 43
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("%1 = add i32 42, 43")));
}

TEST(DeadCodeEliminationTest, test2) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    %1 = add i32 42, 43
    %2 = add i32 %1, %1
    %3 = add i32 %2, %2
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("%1 = add i32 42, 43")),
        Not(HasSubstr("%2 = add i32 %1, %1")),
        Not(HasSubstr("%3 = add i32 %2, %2"))
    ));
}

TEST(DeadCodeEliminationTest, test3) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    %1 = add i32 42, 43
    br label %2

2:
    %3 = add i32 %1, %1
    br label %4

4:
    %5 = add i32 %3, %3
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("%1 = add i32 42, 43")),
        Not(HasSubstr("%3 = add i32 %1, %1")),
        Not(HasSubstr("%5 = add i32 %3, %3"))
    ));
}

TEST(DeadCodeEliminationTest, test4) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    %1 = alloca i32
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("%1 = alloca i32")));
}

TEST(DeadCodeEliminationTest, test5) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    %1 = alloca i32
    store i32 42, ptr %1
    %2 = load i32, ptr %1
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("%2 = load i32, ptr %1")));
}

TEST(DeadCodeEliminationTest, test6) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %3

2:
    br label %4

3:
    br label %4

4:
    %5 = phi i32 [ 42, %2 ], [ 43, %3 ]
    ret void
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("%5 = phi i32 [ 42, %2 ], [ 43, %3 ]")));
}

TEST(DeadCodeEliminationTest, test7) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %4

2:
    %3 = phi i32 [ 42, %1 ], [ 43, %4 ]
    br label %4

4:
    %5 = phi i32 [ 42, %1 ], [ 43, %2 ]
    br label %2
}
)");

    EXPECT_TRUE(DeadCodeElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("%3 = phi i32 [ 42, %1 ], [ 43, %4 ]")),
        Not(HasSubstr("%5 = phi i32 [ 42, %1 ], [ 43, %2 ]"))
    ));
}
