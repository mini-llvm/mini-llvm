// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Instruction/FAdd.h"
#include "mini-llvm/ir/Type/Double.h"

namespace mini_llvm::ir {

class FAddTest : public ::testing::Test {
protected:
    std::shared_ptr<FAdd> fadd_;

    FAddTest()
        : fadd_(std::make_shared<FAdd>(std::make_shared<DoubleConstant>(3.14),
                                       std::make_shared<DoubleConstant>(2.72))) {
        fadd_->setName("result");
    }
};

TEST_F(FAddTest, fold) {
    EXPECT_EQ(*fadd_->fold(), DoubleConstant(3.14 + 2.72));
}

TEST_F(FAddTest, type) {
    EXPECT_EQ(*fadd_->type(), Double());
}

TEST_F(FAddTest, format) {
    EXPECT_EQ(fadd_->format(), "%result = fadd double 0x40091eb851eb851f, 0x4005c28f5c28f5c3");
}

TEST_F(FAddTest, clone) {
    std::shared_ptr<FAdd> cloned = cast<FAdd>(fadd_->clone());
    EXPECT_EQ(&*fadd_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*fadd_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
