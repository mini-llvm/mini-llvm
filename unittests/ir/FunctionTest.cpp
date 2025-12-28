// SPDX-License-Identifier: MIT

#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/CondBr.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

TEST(FunctionTest, IsWellFormedEntryPredecessors) {
    // define void @test() {
    // B1:
    //   br label %B1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<Void>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    BasicBlock &B1 = F.append();
    B1.append(std::make_shared<Br>(weaken(B1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedIncomingBlocks1) {
    // define i32 @test(i1 %arg1) {
    // B1:
    //   br i1 %arg1, label %B2, label %B3
    //
    // B2:
    //   br label %B4
    //
    // B3:
    //   br label %B4
    //
    // B4:
    //   %I1 = phi i32 [ 2, %B2 ], [ 3, %B3 ]
    //   ret i32 %I1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I1>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    BasicBlock &B4 = F.append();
    B1.append(std::make_shared<CondBr>(share(arg1), weaken(B2), weaken(B3)));
    B2.append(std::make_shared<Br>(weaken(B4)));
    B3.append(std::make_shared<Br>(weaken(B4)));
    Phi &I1 = B4.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I1.addIncoming(weaken(B2), std::make_shared<I32Constant>(2));
    I1.addIncoming(weaken(B3), std::make_shared<I32Constant>(3));
    B4.append(std::make_shared<Ret>(share(I1)));

    EXPECT_TRUE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedIncomingBlocks2) {
    // define i32 @test(i1 %arg1) {
    // B1:
    //   br i1 %arg1, label %B2, label %B3
    //
    // B2:
    //   br label %B4
    //
    // B3:
    //   br label %B4
    //
    // B4:
    //   %I1 = phi i32 [ 1, %B1 ], [ 3, %B3 ]
    //   ret i32 %I1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I1>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    BasicBlock &B4 = F.append();
    B1.append(std::make_shared<CondBr>(share(arg1), weaken(B2), weaken(B3)));
    B2.append(std::make_shared<Br>(weaken(B4)));
    B3.append(std::make_shared<Br>(weaken(B4)));
    Phi &I1 = B4.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I1.addIncoming(weaken(B1), std::make_shared<I32Constant>(1));
    I1.addIncoming(weaken(B3), std::make_shared<I32Constant>(3));
    B4.append(std::make_shared<Ret>(share(I1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedIncomingBlocks3) {
    // define i32 @test(i1 %arg1) {
    // B1:
    //   br i1 %arg1, label %B2, label %B3
    //
    // B2:
    //   br label %B4
    //
    // B3:
    //   br label %B4
    //
    // B4:
    //   %I1 = phi i32 [ 2, %B2 ], [ 3, %B3 ], [ 3, %B3 ]
    //   ret i32 %I1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I1>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    BasicBlock &B4 = F.append();
    B1.append(std::make_shared<CondBr>(share(arg1), weaken(B2), weaken(B3)));
    B2.append(std::make_shared<Br>(weaken(B4)));
    B3.append(std::make_shared<Br>(weaken(B4)));
    Phi &I1 = B4.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I1.addIncoming(weaken(B2), std::make_shared<I32Constant>(2));
    I1.addIncoming(weaken(B3), std::make_shared<I32Constant>(3));
    I1.addIncoming(weaken(B3), std::make_shared<I32Constant>(3));
    B4.append(std::make_shared<Ret>(share(I1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedUnreachableBlock1) {
    // define i32 @test(i32 %arg1) {
    // B1:
    //   br label %B3
    //
    // B2:
    //   %I1 = add i32 %arg1, 1
    //   br label %B3
    //
    // B3:
    //   %I2 = phi i32 [ 0, %B1 ],  [ %I1, %B2 ]
    //   ret i32 %I2
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I32>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    B1.append(std::make_shared<Br>(weaken(B3)));
    Add &I1 = B2.append(std::make_shared<Add>(share(arg1), std::make_shared<I32Constant>(1)));
    B2.append(std::make_shared<Br>(weaken(B3)));
    Phi &I2 = B3.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I2.addIncoming(weaken(B1), std::make_shared<I32Constant>(0));
    I2.addIncoming(weaken(B2), share(I1));
    B3.append(std::make_shared<Ret>(share(I2)));

    EXPECT_TRUE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedUnreachableBlock2) {
    // define i32 @test(i32 %arg1) {
    // B1:
    //   br label %B3
    //
    // B2:
    //   %I1 = add i32 %arg1, 1
    //   br label %B3
    //
    // B3:
    //   ret i32 %I1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I32>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    B1.append(std::make_shared<Br>(weaken(B3)));
    Add &I1 = B2.append(std::make_shared<Add>(share(arg1), std::make_shared<I32Constant>(1)));
    B2.append(std::make_shared<Br>(weaken(B3)));
    B3.append(std::make_shared<Ret>(share(I1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedDominance) {
    // define i32 @test(i1 %arg1, i32 %arg2) {
    // B1:
    //   br i1 %arg1, label %B2, label %B3
    //
    // B2:
    //   %I1 = add i32 %arg2, 1
    //   br label %B4
    //
    // B3:
    //   br label %B4
    //
    // B4:
    //   ret i32 %I1
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I1>());
    paramTypes.push_back(std::make_unique<I32>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    Argument &arg2 = F.arg(1);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    BasicBlock &B4 = F.append();
    B1.append(std::make_shared<CondBr>(share(arg1), weaken(B2), weaken(B3)));
    Add &I1 = B2.append(std::make_shared<Add>(share(arg2), std::make_shared<I32Constant>(1)));
    B2.append(std::make_shared<Br>(weaken(B4)));
    B3.append(std::make_shared<Br>(weaken(B4)));
    B4.append(std::make_shared<Ret>(share(I1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedIncomingsDominance1) {
    // define i32 @test(i32 %arg1) {
    // B1:
    //   br label %B2
    //
    // B2:
    //   %I1 = phi i32 [ %I2, %B1 ]
    //   ret i32 %I1
    //
    // B3:
    //   %I2 = add i32 %arg1, 1
    //   ret i32 0
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I32>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    B1.append(std::make_shared<Br>(weaken(B2)));
    Add &I2 = B3.append(std::make_shared<Add>(share(arg1), std::make_shared<I32Constant>(1)));
    B3.append(std::make_shared<Ret>(std::make_shared<I32Constant>(0)));
    Phi &I1 = B2.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I1.addIncoming(weaken(B1), share(I2));
    B2.append(std::make_shared<Ret>(share(I1)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedIncomingsDominance2) {
    // define i32 @test(i1 %arg1, i32 %arg2) {
    // B1:
    //   br i1 %arg1, label %B2, label %B3
    //
    // B2:
    //   %I1 = add i32 %arg2, 1
    //   br label %B4
    //
    // B3:
    //   br label %B4
    //
    // B4:
    //   %I2 = phi i32 [ %I1, %B2 ], [ %I1, %B3 ]
    //   ret i32 %I2
    // }

    std::unique_ptr<Type> returnType = std::make_unique<I32>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    paramTypes.push_back(std::make_unique<I1>());
    paramTypes.push_back(std::make_unique<I32>());
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    Argument &arg1 = F.arg(0);
    Argument &arg2 = F.arg(1);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    BasicBlock &B3 = F.append();
    BasicBlock &B4 = F.append();
    B1.append(std::make_shared<CondBr>(share(arg1), weaken(B2), weaken(B3)));
    Add &I1 = B2.append(std::make_shared<Add>(share(arg2), std::make_shared<I32Constant>(1)));
    B2.append(std::make_shared<Br>(weaken(B4)));
    B3.append(std::make_shared<Br>(weaken(B4)));
    Phi &I2 = B4.append(std::make_shared<Phi>(std::make_unique<I32>()));
    I2.addIncoming(weaken(B2), share(I1));
    I2.addIncoming(weaken(B3), share(I1));
    B4.append(std::make_shared<Ret>(share(I2)));

    EXPECT_FALSE(F.isWellFormed());
}

TEST(FunctionTest, IsWellFormedAlloca) {
    // define void @test() {
    // B1:
    //   br label %B2
    //
    // B2:
    //   %I1 = alloca i32
    //   br label %B2
    // }

    std::unique_ptr<Type> returnType = std::make_unique<Void>();
    std::vector<std::unique_ptr<Type>> paramTypes;
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::move(returnType), std::move(paramTypes), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    BasicBlock &B1 = F.append();
    BasicBlock &B2 = F.append();
    B1.append(std::make_shared<Br>(weaken(B2)));
    B2.append(std::make_shared<Alloca>(std::make_unique<I32>()));
    B2.append(std::make_shared<Br>(weaken(B2)));

    EXPECT_FALSE(F.isWellFormed());
}

} // namespace mini_llvm::ir
