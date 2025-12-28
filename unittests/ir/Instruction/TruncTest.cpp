// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class TruncTest : public ::testing::Test {
protected:
    std::shared_ptr<Trunc> trunc_;

    TruncTest() : trunc_(std::make_shared<Trunc>(std::make_shared<I64Constant>(40926266145), std::make_unique<I32>())) {
        trunc_->setName("result");
    }
};

TEST_F(TruncTest, fold) {
    EXPECT_EQ(*trunc_->fold(), I32Constant(-2023406815));
}

TEST_F(TruncTest, type) {
    EXPECT_EQ(*trunc_->type(), I32());
}

TEST_F(TruncTest, format) {
    EXPECT_EQ(trunc_->format(), "%result = trunc i64 40926266145 to i32");
}

TEST_F(TruncTest, clone) {
    std::shared_ptr<Trunc> cloned = cast<Trunc>(trunc_->clone());
    EXPECT_EQ(&*trunc_->value(), &*cloned->value());
    EXPECT_EQ(*trunc_->type(), *cloned->type());
}

} // namespace mini_llvm::ir
