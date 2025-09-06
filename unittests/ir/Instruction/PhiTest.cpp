// SPDX-License-Identifier: MIT

#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

class PhiTest : public ::testing::Test {
protected:
    std::shared_ptr<BasicBlock> block1_, block2_;
    std::shared_ptr<Phi> phi_;

    PhiTest() : block1_(std::make_shared<BasicBlock>()), block2_(std::make_shared<BasicBlock>()),
                phi_(std::make_shared<Phi>(std::make_unique<I32>())) {
        block1_->setName("block1");
        block2_->setName("block2");
        phi_->setName("result");
        phi_->addIncoming(block1_, std::make_shared<I32Constant>(42));
        phi_->addIncoming(block2_, std::make_shared<I32Constant>(43));
    }
};

TEST_F(PhiTest, type) {
    EXPECT_EQ(*phi_->type(), I32());
}

TEST_F(PhiTest, format) {
    std::string result = phi_->format();
    EXPECT_EQ(result, "%result = phi i32 [ 42, %block1 ], [ 43, %block2 ]");
}

TEST_F(PhiTest, clone) {
    std::shared_ptr<Phi> cloned = cast<Phi>(phi_->clone());
    EXPECT_EQ(*phi_->type(), *cloned->type());
    for (Phi::ConstIncoming incoming : incomings(*phi_)) {
        EXPECT_TRUE(hasIncomingBlock(*cloned, *incoming.block));
        EXPECT_EQ(&*getIncomingValue(*cloned, *incoming.block), &*incoming.value);
    }
    for (Phi::ConstIncoming incoming : incomings(*cloned)) {
        EXPECT_TRUE(hasIncomingBlock(*phi_, *incoming.block));
        EXPECT_EQ(&*getIncomingValue(*phi_, *incoming.block), &*incoming.value);
    }
}

} // namespace mini_llvm::ir
