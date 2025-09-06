// SPDX-License-Identifier: MIT

#include <memory>
#include <unordered_set>

#include <gtest/gtest.h>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Type/Void.h"

namespace mini_llvm::ir {

class BrTest : public ::testing::Test {
protected:
    std::shared_ptr<BasicBlock> block_;
    std::shared_ptr<Br> br_;

    BrTest() : block_(std::make_shared<BasicBlock>()), br_(std::make_shared<Br>(block_)) {
        block_->setName("block");
    }
};

TEST_F(BrTest, successors) {
    EXPECT_EQ(br_->successors(), std::unordered_set<BasicBlock *>{&*br_->dest()});
}

TEST_F(BrTest, type) {
    EXPECT_EQ(*br_->type(), Void());
}

TEST_F(BrTest, format) {
    EXPECT_EQ(br_->format(), "br label %block");
}

TEST_F(BrTest, clone) {
    std::shared_ptr<Br> cloned = cast<Br>(br_->clone());
    EXPECT_EQ(&*br_->dest(), &*cloned->dest());
}

} // namespace mini_llvm::ir
