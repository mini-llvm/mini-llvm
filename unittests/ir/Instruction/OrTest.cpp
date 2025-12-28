// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Or.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class OrTest : public ::testing::Test {
protected:
    std::shared_ptr<Or> or_;

    OrTest() : or_(std::make_shared<Or>(std::make_shared<I32Constant>(171), std::make_shared<I32Constant>(205))) {
        or_->setName("result");
    }
};

TEST_F(OrTest, fold) {
    EXPECT_EQ(*or_->fold(), I32Constant(239));
}

TEST_F(OrTest, type) {
    EXPECT_EQ(*or_->type(), I32());
}

TEST_F(OrTest, format) {
    EXPECT_EQ(or_->format(), "%result = or i32 171, 205");
}

TEST_F(OrTest, clone) {
    std::shared_ptr<Or> cloned = cast<Or>(or_->clone());
    EXPECT_EQ(&*or_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*or_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
