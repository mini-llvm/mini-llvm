// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class LoadTest : public ::testing::Test {
protected:
    std::shared_ptr<Alloca> alloca_;
    std::shared_ptr<Load> load_;

    LoadTest() : alloca_(std::make_shared<Alloca>(std::make_unique<I32>())),
                 load_(std::make_shared<Load>(std::make_unique<I32>(), alloca_)) {
        alloca_->setName("ptr");
        load_->setName("result");
    }
};

TEST_F(LoadTest, type) {
    EXPECT_EQ(*load_->type(), I32());
}

TEST_F(LoadTest, format) {
    EXPECT_EQ(load_->format(), "%result = load i32, ptr %ptr");
}

TEST_F(LoadTest, clone) {
    std::shared_ptr<Load> cloned = cast<Load>(load_->clone());
    EXPECT_EQ(*load_->type(), *cloned->type());
    EXPECT_EQ(&*load_->ptr(), &*cloned->ptr());
}

} // namespace mini_llvm::ir
