// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class SelectTest : public ::testing::Test {
protected:
    std::shared_ptr<Select> select_;

    SelectTest()
        : select_(std::make_shared<Select>(
            std::make_shared<I1Constant>(true),
            std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        select_->setName("result");
    }
};

TEST_F(SelectTest, fold) {
    EXPECT_EQ(*select_->fold(), I32Constant(42));
}

TEST_F(SelectTest, type) {
    EXPECT_EQ(*select_->type(), I32());
}

TEST_F(SelectTest, format) {
    EXPECT_EQ(select_->format(), "%result = select i1 true, i32 42, i32 43");
}

TEST_F(SelectTest, clone) {
    std::shared_ptr<Select> cloned = cast<Select>(select_->clone());
    EXPECT_EQ(&*select_->cond(), &*cloned->cond());
    EXPECT_EQ(&*select_->trueValue(), &*cloned->trueValue());
    EXPECT_EQ(&*select_->falseValue(), &*cloned->falseValue());
}

} // namespace mini_llvm::ir
