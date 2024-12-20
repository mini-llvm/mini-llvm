#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/IRParser.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Not;

using namespace mini_llvm;
using namespace mini_llvm::ir;

TEST(UnreachableBlockEliminationTest, test0) {
    std::shared_ptr<Function> F = share(parseModule(R"(
define void @foo() {
0:
    ret void
}
)").value().functions.front());

    EXPECT_FALSE(UnreachableBlockElimination().runOnFunction(*F));
}

TEST(UnreachableBlockEliminationTest, test1) {
    std::shared_ptr<Function> F = share(parseModule(R"(
define void @foo() {
0:
    ret void

1:
    ret void
}
)").value().functions.front());

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));

    VerificationAnalysis verify;
    verify.runOnFunction(*F);
    EXPECT_TRUE(verify.ok());

    EXPECT_THAT(F->format(), Not(HasSubstr("1:")));
}

TEST(UnreachableBlockEliminationTest, test2) {
    std::shared_ptr<Function> F = share(parseModule(R"(
define void @foo() {
0:
    ret void

1:
    br label %1
}
)").value().functions.front());

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));

    VerificationAnalysis verify;
    verify.runOnFunction(*F);
    EXPECT_TRUE(verify.ok());

    EXPECT_THAT(F->format(), Not(HasSubstr("1:")));
}

TEST(UnreachableBlockEliminationTest, test3) {
    std::shared_ptr<Function> F = share(parseModule(R"(
define void @foo() {
0:
    ret void

1:
    br label %2

2:
    br label %1
}
)").value().functions.front());

    EXPECT_TRUE(UnreachableBlockElimination().runOnFunction(*F));

    VerificationAnalysis verify;
    verify.runOnFunction(*F);
    EXPECT_TRUE(verify.ok());

    EXPECT_THAT(F->format(), AllOf(
        Not(HasSubstr("1:")),
        Not(HasSubstr("2:"))
    ));
}
