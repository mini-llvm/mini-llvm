#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/opt/ir/passes/ConstantFolding.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using ::testing::HasSubstr;

using namespace mini_llvm::ir;

TEST(ConstantFoldingTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void
}
)");

    EXPECT_FALSE(ConstantFolding().runOnFunction(*F));
}

TEST(ConstantFoldingTest, test1) {
    std::shared_ptr<Function> F = parseFunction(R"(
define i32 @foo() {
0:
    %1 = add i32 42, 43
    ret i32 %1
}
)");

    EXPECT_TRUE(ConstantFolding().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("ret i32 85"));
}

TEST(ConstantFoldingTest, test2) {
    std::shared_ptr<Function> F = parseFunction(R"(
define i32 @foo() {
0:
    %1 = add i32 42, 43
    %2 = add i32 %1, 44
    %3 = add i32 %2, 45
    ret i32 %3
}
)");

    EXPECT_TRUE(ConstantFolding().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("ret i32 174"));
}

TEST(ConstantFoldingTest, test3) {
    std::shared_ptr<Function> F = parseFunction(R"(
define i32 @foo() {
0:
    br label %5

2:
    ret i32 %4

3:
    %4 = add i32 %6, 44
    br label %2

5:
    %6 = add i32 42, 43
    br label %3
}
)");

    EXPECT_TRUE(ConstantFolding().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("ret i32 129"));
}
