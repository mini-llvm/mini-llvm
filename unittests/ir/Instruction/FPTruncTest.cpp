// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Type/Float.h"

namespace mini_llvm::ir {

class FPTruncTest : public ::testing::Test {
protected:
    std::shared_ptr<FPTrunc> fptrunc_;

    FPTruncTest() : fptrunc_(std::make_shared<FPTrunc>(std::make_shared<DoubleConstant>(3.14),
                                                       std::make_unique<Float>())) {
        fptrunc_->setName("result");
    }
};

TEST_F(FPTruncTest, fold) {
    EXPECT_EQ(*fptrunc_->fold(), FloatConstant(static_cast<float>(3.14)));
}

TEST_F(FPTruncTest, type) {
    EXPECT_EQ(*fptrunc_->type(), Float());
}

TEST_F(FPTruncTest, format) {
    EXPECT_EQ(fptrunc_->format(), "%result = fptrunc double 0x40091eb851eb851f to float");
}

TEST_F(FPTruncTest, clone) {
    std::shared_ptr<FPTrunc> cloned = cast<FPTrunc>(fptrunc_->clone());
    EXPECT_EQ(&*fptrunc_->value(), &*cloned->value());
    EXPECT_EQ(*fptrunc_->type(), *cloned->type());
}

} // namespace mini_llvm::ir
