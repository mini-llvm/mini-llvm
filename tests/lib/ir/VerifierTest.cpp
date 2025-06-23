#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/VoidValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/ir/Verifier.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

TEST(VerifierTest, SelfReferentialInstruction) {
    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
        std::make_unique<Void>(), std::vector<std::unique_ptr<Type>>(), false
    );
    Function F(std::move(functionType), Linkage::kExternal);
    BasicBlock &B = F.append();
    std::shared_ptr<Value> op = std::make_shared<I32Constant>(0);
    Add &I = B.append(std::make_shared<Add>(op, op));
    replaceAllUsesWith(*op, share(I));
    B.append(std::make_shared<Ret>(std::make_shared<VoidValue>()));
    EXPECT_FALSE(verifyFunction(F));
}
