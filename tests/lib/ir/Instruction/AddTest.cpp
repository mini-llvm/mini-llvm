#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Type/I32.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

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
