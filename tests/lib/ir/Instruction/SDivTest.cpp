#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/SDiv.h"
#include "mini-llvm/ir/Type/I32.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

class SDivTest : public ::testing::Test {
protected:
    std::shared_ptr<SDiv> sdiv_;

    SDivTest() : sdiv_(std::make_shared<SDiv>(std::make_shared<I32Constant>(42), std::make_shared<I32Constant>(43))) {
        sdiv_->setName("result");
    }
};

TEST_F(SDivTest, type) {
    EXPECT_EQ(*sdiv_->type(), I32());
}

TEST_F(SDivTest, format) {
    EXPECT_EQ(sdiv_->format(), "%result = sdiv i32 42, 43");
}

TEST_F(SDivTest, clone) {
    std::shared_ptr<SDiv> cloned = cast<SDiv>(sdiv_->clone());
    EXPECT_EQ(&*sdiv_->lhs(), &*cloned->lhs());
    EXPECT_EQ(&*sdiv_->rhs(), &*cloned->rhs());
}
