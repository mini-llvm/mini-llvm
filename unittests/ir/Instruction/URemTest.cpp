// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/URem.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class URemTest : public ::testing::Test {
protected:
    std::shared_ptr<URem> urem_;

    URemTest() : urem_(std::make_shared<URem>(std::make_shared<I32Constant>(-100), std::make_shared<I32Constant>(30))) {
        urem_->setName("result");
    }
};

TEST_F(URemTest, fold) {
    EXPECT_EQ(*urem_->fold(), I32Constant(6));
}

TEST_F(URemTest, type) {
    EXPECT_EQ(*urem_->type(), I32());
}

TEST_F(URemTest, format) {
    EXPECT_EQ(urem_->format(), "%result = urem i32 -100, 30");
}

TEST_F(URemTest, clone) {
    std::shared_ptr<URem> cloned = cast<URem>(urem_->clone());
    EXPECT_EQ(&*urem_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*urem_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
