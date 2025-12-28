// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/Ptr.h"

namespace mini_llvm::ir {

class AllocaTest : public ::testing::Test {
protected:
    std::shared_ptr<Alloca> alloca_;

    AllocaTest() : alloca_(std::make_shared<Alloca>(std::make_unique<I32>())) {
        alloca_->setName("ptr");
    }
};

TEST_F(AllocaTest, type) {
    EXPECT_EQ(*alloca_->type(), Ptr());
}

TEST_F(AllocaTest, format) {
    EXPECT_EQ(alloca_->format(), "%ptr = alloca i32");
}

TEST_F(AllocaTest, clone) {
    std::shared_ptr<Alloca> cloned = cast<Alloca>(alloca_->clone());
    EXPECT_EQ(*alloca_->allocatedType(), *cloned->allocatedType());
}

} // namespace mini_llvm::ir
