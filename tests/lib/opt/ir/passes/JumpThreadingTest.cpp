#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/opt/ir/passes/JumpThreading.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using ::testing::AllOf;
using ::testing::HasSubstr;

using namespace mini_llvm::ir;

TEST(JumpThreadingTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void
}
)");

    EXPECT_FALSE(JumpThreading().runOnFunction(*F));
}

TEST(JumpThreadingTest, test1) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    br label %1

1:
    br label %2

2:
    ret void
}
)");

    EXPECT_TRUE(JumpThreading().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("br label %1")));
}

TEST(JumpThreadingTest, test2) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %4

2:
    br label %3

3:
    ret void

4:
    br label %5

5:
    ret void
}
)");

    EXPECT_TRUE(JumpThreading().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("br i1 %0, label %2, label %4")));
}

TEST(JumpThreadingTest, test3) {
    std::shared_ptr<Function> F = parseFunction(R"(
define i32 @foo() {
0:
    br label %1

1:
    br label %2

2:
    br label %3

3:
    %4 = phi i32 [ 42, %2 ]
    ret i32 %4
}
)");

    EXPECT_TRUE(JumpThreading().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("br label %1")),
        Not(HasSubstr("br label %2")),
        HasSubstr("[ 42, %0 ]"),
        HasSubstr("[ 42, %1 ]")
    ));
}

TEST(JumpThreadingTest, test4) {
    std::shared_ptr<Function> F = parseFunction(R"(
define i32 @foo(i1 %0) {
1:
    br i1 %0, label %2, label %3

2:
    br label %3

3:
    %4 = phi i32 [ 42, %1 ], [ 43, %2 ]
    ret i32 %4
}
)");

    EXPECT_FALSE(JumpThreading().runOnFunction(*F));
}
