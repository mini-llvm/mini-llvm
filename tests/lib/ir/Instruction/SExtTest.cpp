#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Type/I64.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

class SExtTest : public ::testing::Test {
protected:
    std::shared_ptr<SExt> sext_;

    SExtTest() : sext_(std::make_shared<SExt>(std::make_shared<I32Constant>(42), std::make_unique<I64>())) {
        sext_->setName("result");
    }
};

TEST_F(SExtTest, type) {
    EXPECT_EQ(*sext_->type(), I64());
}

TEST_F(SExtTest, format) {
    EXPECT_EQ(sext_->format(), "%result = sext i32 42 to i64");
}

TEST_F(SExtTest, clone) {
    std::shared_ptr<SExt> cloned = cast<SExt>(sext_->clone());
    EXPECT_EQ(&*sext_->value(), &*cloned->value());
    EXPECT_EQ(*sext_->type(), *cloned->type());
}
