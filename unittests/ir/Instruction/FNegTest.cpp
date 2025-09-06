// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Instruction/FNeg.h"
#include "mini-llvm/ir/Type/Double.h"

namespace mini_llvm::ir {

class FNegTest : public ::testing::Test {
protected:
    std::shared_ptr<FNeg> fneg_;

    FNegTest() : fneg_(std::make_shared<FNeg>(std::make_shared<DoubleConstant>(3.14))) {
        fneg_->setName("result");
    }
};

TEST_F(FNegTest, fold) {
    EXPECT_EQ(*fneg_->fold(), DoubleConstant(-3.14));
}

TEST_F(FNegTest, type) {
    EXPECT_EQ(*fneg_->type(), Double());
}

TEST_F(FNegTest, format) {
    EXPECT_EQ(fneg_->format(), "%result = fneg double 0x40091eb851eb851f");
}

TEST_F(FNegTest, clone) {
    std::shared_ptr<FNeg> cloned = cast<FNeg>(fneg_->clone());
    EXPECT_EQ(&*fneg_->value(), &*cloned->value());
}

} // namespace mini_llvm::ir
