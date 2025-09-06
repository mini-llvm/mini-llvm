// SPDX-License-Identifier: MIT

#include <cmath>
#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Instruction/FRem.h"
#include "mini-llvm/ir/Type/Double.h"

namespace mini_llvm::ir {

class FRemTest : public ::testing::Test {
protected:
    std::shared_ptr<FRem> frem_;

    FRemTest() : frem_(std::make_shared<FRem>(std::make_shared<DoubleConstant>(3.14),
                                              std::make_shared<DoubleConstant>(2.72))) {
        frem_->setName("result");
    }
};

TEST_F(FRemTest, fold) {
    EXPECT_EQ(*frem_->fold(), DoubleConstant(fmod(3.14, 2.72)));
}

TEST_F(FRemTest, type) {
    EXPECT_EQ(*frem_->type(), Double());
}

TEST_F(FRemTest, format) {
    EXPECT_EQ(frem_->format(), "%result = frem double 0x40091eb851eb851f, 0x4005c28f5c28f5c3");
}

TEST_F(FRemTest, clone) {
    std::shared_ptr<FRem> cloned = cast<FRem>(frem_->clone());
    EXPECT_EQ(&*frem_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*frem_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
