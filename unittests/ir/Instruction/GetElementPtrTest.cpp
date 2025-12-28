// SPDX-License-Identifier: MIT

#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class GetElementPtrTest : public ::testing::Test {
protected:
    std::shared_ptr<GetElementPtr> getelememtptr_;

    GetElementPtrTest() :
        getelememtptr_(
            std::make_shared<GetElementPtr>(std::make_unique<I32>(),
                                            std::make_shared<NullPtrConstant>(),
                                            std::vector<std::shared_ptr<Value>>{std::make_shared<I64Constant>(2)})) {
        getelememtptr_->setName("result");
    }
};

TEST_F(GetElementPtrTest, type) {
    EXPECT_EQ(*getelememtptr_->type(), Ptr());
}

TEST_F(GetElementPtrTest, format) {
    EXPECT_EQ(getelememtptr_->format(), "%result = getelementptr i32, ptr null, i64 2");
}

TEST_F(GetElementPtrTest, clone) {
    std::shared_ptr<GetElementPtr> cloned = cast<GetElementPtr>(getelememtptr_->clone());
    EXPECT_EQ(*cloned->type(), *getelememtptr_->type());
    EXPECT_EQ(&*cloned->ptr(), &*getelememtptr_->ptr());
    EXPECT_EQ(cloned->idx_size(), getelememtptr_->idx_size());
    EXPECT_EQ(&*cloned->idx(0), &*getelememtptr_->idx(0));
}

} // namespace mini_llvm::ir
