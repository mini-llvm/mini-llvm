// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class SHLTest : public ::testing::Test {
protected:
    std::shared_ptr<SHL> shl_;

    SHLTest() : shl_(std::make_shared<SHL>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(2))) {
        shl_->setName("result");
    }
};

TEST_F(SHLTest, fold) {
    EXPECT_EQ(*shl_->fold(), I32Constant(168));
}

TEST_F(SHLTest, type) {
    EXPECT_EQ(*shl_->type(), I32());
}

TEST_F(SHLTest, format) {
    EXPECT_EQ(shl_->format(), "%result = shl i32 42, 2");
}

TEST_F(SHLTest, clone) {
    std::shared_ptr<SHL> cloned = cast<SHL>(shl_->clone());
    EXPECT_EQ(&*shl_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*shl_->rhs(), &*cloned->rhs());
}

} // namespace mini_llvm::ir
