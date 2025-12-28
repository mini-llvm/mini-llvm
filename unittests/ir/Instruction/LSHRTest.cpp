// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class LSHRTest : public ::testing::Test {
protected:
    std::shared_ptr<LSHR> lshr_;

    LSHRTest() : lshr_(std::make_shared<LSHR>(std::make_shared<I32Constant>(-42), std::make_shared<I32Constant>(2))) {
        lshr_->setName("result");
    }
};

TEST_F(LSHRTest, fold) {
    EXPECT_EQ(*lshr_->fold(), I32Constant(1073741813));
}

TEST_F(LSHRTest, type) {
    EXPECT_EQ(*lshr_->type(), I32());
}

TEST_F(LSHRTest, format) {
    EXPECT_EQ(lshr_->format(), "%result = lshr i32 -42, 2");
}

TEST_F(LSHRTest, clone) {
    std::shared_ptr<LSHR> cloned = cast<LSHR>(lshr_->clone());
    EXPECT_EQ(&*lshr_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*lshr_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
