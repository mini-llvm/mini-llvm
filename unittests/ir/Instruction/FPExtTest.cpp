// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Type/Double.h"

namespace mini_llvm::ir {

class FPExtTest : public ::testing::Test {
protected:
    std::shared_ptr<FPExt> fpext_;

    FPExtTest() : fpext_(std::make_shared<FPExt>(std::make_shared<FloatConstant>(3.14f),
                                                 std::make_unique<Double>())) {
        fpext_->setName("result");
    }
};

TEST_F(FPExtTest, fold) {
    EXPECT_EQ(*fpext_->fold(), DoubleConstant(static_cast<double>(3.14f)));
}

TEST_F(FPExtTest, type) {
    EXPECT_EQ(*fpext_->type(), Double());
}

TEST_F(FPExtTest, format) {
    EXPECT_EQ(fpext_->format(), "%result = fpext float 0x40091eb860000000 to double");
}

TEST_F(FPExtTest, clone) {
    std::shared_ptr<FPExt> cloned = cast<FPExt>(fpext_->clone());
    EXPECT_EQ(&*fpext_->value(), &*cloned->value());
    EXPECT_EQ(*fpext_->type(), *cloned->type());
}

} // namespace mini_llvm::ir
