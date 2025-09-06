// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Instruction/FMul.h"
#include "mini-llvm/ir/Type/Double.h"

namespace mini_llvm::ir {

class FMulTest : public ::testing::Test {
protected:
    std::shared_ptr<FMul> fmul_;

    FMulTest() : fmul_(std::make_shared<FMul>(std::make_shared<DoubleConstant>(3.14),
                                              std::make_shared<DoubleConstant>(2.72))) {
        fmul_->setName("result");
    }
};

TEST_F(FMulTest, fold) {
    EXPECT_EQ(*fmul_->fold(), DoubleConstant(3.14 * 2.72));
}

TEST_F(FMulTest, type) {
    EXPECT_EQ(*fmul_->type(), Double());
}

TEST_F(FMulTest, format) {
    EXPECT_EQ(fmul_->format(), "%result = fmul double 0x40091eb851eb851f, 0x4005c28f5c28f5c3");
}

TEST_F(FMulTest, clone) {
    std::shared_ptr<FMul> cloned = cast<FMul>(fmul_->clone());
    EXPECT_EQ(&*fmul_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*fmul_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
