// SPDX-License-Identifier: MIT

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I8.h"

namespace mini_llvm::ir {

TEST(ArrayConstantTest, FormatAsString) {
    std::unique_ptr<ArrayType> type = std::make_unique<ArrayType>(std::make_unique<I8>(), 13);

    std::vector<std::shared_ptr<Constant>> elements;
    for (int8_t element : {32, 126, 65, 90, 97, 122, 92, 48, 57, 0, 31, 127, -1}) {
        elements.push_back(std::make_shared<I8Constant>(element));
    }

    ArrayConstant C(std::move(type), std::move(elements));

    EXPECT_EQ(C.format(), R"(c" ~AZaz\\09\00\1F\7F\FF")");
}

TEST(ArrayConstantTest, FormatAsArray) {
    std::unique_ptr<ArrayType> type = std::make_unique<ArrayType>(std::make_unique<I32>(), 6);

    std::vector<std::shared_ptr<Constant>> elements;
    for (int32_t element : {5, 2, 4, 6, 1, 3}) {
        elements.push_back(std::make_shared<I32Constant>(element));
    }

    ArrayConstant C(std::move(type), std::move(elements));

    EXPECT_EQ(C.format(), "[i32 5, i32 2, i32 4, i32 6, i32 1, i32 3]");
}

TEST(ArrayConstantTest, FormatAsZeroInitializer) {
    std::unique_ptr<ArrayType> type = std::make_unique<ArrayType>(std::make_unique<I32>(), 5);

    std::vector<std::shared_ptr<Constant>> elements;

    ArrayConstant C(std::move(type), std::move(elements));

    EXPECT_EQ(C.format(), "zeroinitializer");
}

} // namespace mini_llvm::ir
