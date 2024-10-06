#include <memory>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/Type/I64.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

class ZExtTest : public ::testing::Test {
protected:
    std::shared_ptr<ZExt> zext_;

    ZExtTest() : zext_(std::make_shared<ZExt>(std::make_shared<I32Constant>(42), std::make_unique<I64>())) {
        zext_->setName("result");
    }
};

TEST_F(ZExtTest, type) {
    EXPECT_EQ(*zext_->type(), I64());
}

TEST_F(ZExtTest, format) {
    EXPECT_EQ(zext_->format(), "%result = zext i32 42 to i64");
}

TEST_F(ZExtTest, clone) {
    std::shared_ptr<ZExt> cloned = cast<ZExt>(zext_->clone());
    EXPECT_EQ(&*zext_->value(), &*cloned->value());
    EXPECT_EQ(*zext_->type(), *cloned->type());
}
