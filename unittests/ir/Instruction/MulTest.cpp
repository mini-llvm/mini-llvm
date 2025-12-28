// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class MulTest : public ::testing::Test {
protected:
    std::shared_ptr<Mul> mul_;

    MulTest() : mul_(std::make_shared<Mul>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        mul_->setName("result");
    }
};

TEST_F(MulTest, fold) {
    EXPECT_EQ(*mul_->fold(), I32Constant(1806));
}

TEST_F(MulTest, type) {
    EXPECT_EQ(*mul_->type(), I32());
}

TEST_F(MulTest, format) {
    EXPECT_EQ(mul_->format(), "%result = mul i32 42, 43");
}

TEST_F(MulTest, clone) {
    std::shared_ptr<Mul> cloned = cast<Mul>(mul_->clone());
    EXPECT_EQ(&*mul_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*mul_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
