#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/opt/ir/passes/BranchSimplification.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using ::testing::HasSubstr;

using namespace mini_llvm::ir;

TEST(BranchSimplificationTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void
}
)");

    EXPECT_FALSE(BranchSimplification().runOnFunction(*F));
}

TEST(BranchSimplificationTest, test1) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    br i1 true, label %1, label %2

1:
    ret void

2:
    ret void
}
)");

    EXPECT_TRUE(BranchSimplification().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("br label %1"));
}

TEST(BranchSimplificationTest, test2) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    br i1 false, label %1, label %2

1:
    ret void

2:
    ret void
}
)");

    EXPECT_TRUE(BranchSimplification().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("br label %2"));
}

TEST(BranchSimplificationTest, test3) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %2

2:
    ret void
}
)");

    EXPECT_TRUE(BranchSimplification().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), HasSubstr("br label %2"));
}
