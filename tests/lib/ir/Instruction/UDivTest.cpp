#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/UDiv.h"
#include "mini-llvm/ir/Type/I32.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

class UDivTest : public ::testing::Test {
protected:
    std::shared_ptr<UDiv> udiv_;

    UDivTest() : udiv_(std::make_shared<UDiv>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        udiv_->setName("result");
    }
};

TEST_F(UDivTest, type) {
    EXPECT_EQ(*udiv_->type(), I32());
}

TEST_F(UDivTest, format) {
    EXPECT_EQ(udiv_->format(), "%result = udiv i32 42, 43");
}

TEST_F(UDivTest, clone) {
    std::shared_ptr<UDiv> cloned = cast<UDiv>(udiv_->clone());
    EXPECT_EQ(&*udiv_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*udiv_->rhs(), &*cloned->rhs());
}
