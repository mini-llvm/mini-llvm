// SPDX-License-Identifier: MIT

#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/VoidValue.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type/I32.h"

namespace mini_llvm::ir {

TEST(BasicBlockTest, IsWellFormedEmpty) {
    BasicBlock B;

    EXPECT_FALSE(B.isWellFormed());
}

TEST(BasicBlockTest, IsWellFormedMissingTerminator) {
    BasicBlock B;
    B.append(std::make_shared<Alloca>(std::make_unique<I32>()));

    EXPECT_FALSE(B.isWellFormed());
}

TEST(BasicBlockTest, IsWellFormedUnexpectedTerminator) {
    BasicBlock B;
    B.append(std::make_shared<Ret>(std::make_shared<VoidValue>()));
    B.append(std::make_shared<Ret>(std::make_shared<VoidValue>()));

    EXPECT_FALSE(B.isWellFormed());
}

} // namespace mini_llvm::ir
