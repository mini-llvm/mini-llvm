// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class SubTest : public ::testing::Test {
protected:
    std::shared_ptr<Sub> sub_;

    SubTest() : sub_(std::make_shared<Sub>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        sub_->setName("result");
    }
};

TEST_F(SubTest, fold) {
    EXPECT_EQ(*sub_->fold(), I32Constant(-1));
}

TEST_F(SubTest, type) {
    EXPECT_EQ(*sub_->type(), I32());
}

TEST_F(SubTest, format) {
    EXPECT_EQ(sub_->format(), "%result = sub i32 42, 43");
}

TEST_F(SubTest, clone) {
    std::shared_ptr<Sub> cloned = cast<Sub>(sub_->clone());
    EXPECT_EQ(&*sub_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*sub_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
