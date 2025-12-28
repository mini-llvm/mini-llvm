// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class AddTest : public ::testing::Test {
protected:
    std::shared_ptr<Add> add_;

    AddTest() : add_(std::make_shared<Add>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        add_->setName("result");
    }
};

TEST_F(AddTest, fold) {
    EXPECT_EQ(*add_->fold(), I32Constant(85));
}

TEST_F(AddTest, type) {
    EXPECT_EQ(*add_->type(), I32());
}

TEST_F(AddTest, format) {
    EXPECT_EQ(add_->format(), "%result = add i32 42, 43");
}

TEST_F(AddTest, clone) {
    std::shared_ptr<Add> cloned = cast<Add>(add_->clone());
    EXPECT_EQ(&*add_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*add_->rhs(), &*cloned->rhs());
}

TEST_F(AddTest, isWellFormed) {
    EXPECT_TRUE(add_->isWellFormed());

    std::shared_ptr<Value> lhs = std::make_shared<I32Constant>(0);
    std::shared_ptr<Value> rhs = std::make_shared<I64Constant>(0);
    add_->lhs().set(lhs);
    add_->rhs().set(rhs);
    EXPECT_FALSE(add_->isWellFormed());

    std::shared_ptr<Value> value = std::make_shared<I32Constant>(0);
    add_->lhs().set(value);
    add_->rhs().set(value);
    replaceAllUsesWith(*value, add_);
    EXPECT_FALSE(add_->isWellFormed());
}

} // namespace mini_llvm::ir
