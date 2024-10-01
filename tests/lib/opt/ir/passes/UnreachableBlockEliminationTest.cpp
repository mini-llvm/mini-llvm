#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/ir_parser.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Not;

using namespace mini_llvm::ir;

TEST(UnreachableBlockEliminationTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void
}
)");

    EXPECT_FALSE(UnreachableBlockElimination().runOnFunction(*F));
}

TEST(UnreachableBlockEliminationTest, test1) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void

1:
    ret void
}
)");

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("1:")));
}

TEST(UnreachableBlockEliminationTest, test2) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void

1:
    br label %1
}
)");

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), Not(HasSubstr("1:")));
}

TEST(UnreachableBlockEliminationTest, test3) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo() {
0:
    ret void

1:
    br label %2

2:
    br label %1
}
)");

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));
    EXPECT_NO_THROW(VerificationAnalysis().runOnFunction(*F));
    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("1:")),
        Not(HasSubstr("2:"))
    ));
}
