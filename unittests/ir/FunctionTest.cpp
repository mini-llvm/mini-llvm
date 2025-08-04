#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

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
