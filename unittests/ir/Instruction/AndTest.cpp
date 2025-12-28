// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/And.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class AndTest : public ::testing::Test {
protected:
    std::shared_ptr<And> and_;

    AndTest() : and_(std::make_shared<And>(std::make_shared<I32Constant>(171), std::make_shared<I32Constant>(205))) {
        and_->setName("result");
    }
};

TEST_F(AndTest, fold) {
    EXPECT_EQ(*and_->fold(), I32Constant(137));
}

TEST_F(AndTest, type) {
    EXPECT_EQ(*and_->type(), I32());
}

TEST_F(AndTest, format) {
    EXPECT_EQ(and_->format(), "%result = and i32 171, 205");
}

TEST_F(AndTest, clone) {
    std::shared_ptr<And> cloned = cast<And>(and_->clone());
    EXPECT_EQ(&*and_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*and_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
