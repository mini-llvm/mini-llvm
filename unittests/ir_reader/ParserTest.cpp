// SPDX-License-Identifier: MIT

#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir_reader/Parser.h"
#include "mini-llvm/ir_reader/Token.h"

using enum mini_llvm::ir::Token::Kind;

namespace mini_llvm::ir {

TEST(ParserTest, Empty) {
    std::vector<Token> input{
        {kEOF, {}, nullptr},
    };

    Parser(input.begin()).parseModule();
}

TEST(ParserTest, ArrayType) {
    // [2 x [3 x [4 x i32]]]
    std::vector<Token> input{
        {kLeftBracket,  {}, nullptr},
        {kNumber,       2,  nullptr},
        {kX,            {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kNumber,       3,  nullptr},
        {kX,            {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kNumber,       4,  nullptr},
        {kX,            {}, nullptr},
        {kI32,          {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kEOF,          {}, nullptr},
    };

    std::unique_ptr<Type> expected = std::make_unique<ArrayType>(
        std::make_unique<ArrayType>(
            std::make_unique<ArrayType>(
                std::make_unique<I32>(),
                4
            ),
            3
        ),
        2
    );

    EXPECT_EQ(*Parser(input.begin()).parseType(), *expected);
}

TEST(ParserTest, ArrayConstant) {
    // [[2 x i32] [i32 42, i32 43], [2 x i32] [i32 44, i32 45]]
    std::vector<Token> input{
        {kLeftBracket,  {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kNumber,       2,  nullptr},
        {kX,            {}, nullptr},
        {kI32,          {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kI32,          {}, nullptr},
        {kNumber,       42, nullptr},
        {kComma,        {}, nullptr},
        {kI32,          {}, nullptr},
        {kNumber,       43, nullptr},
        {kRightBracket, {}, nullptr},
        {kComma,        {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kNumber,       2, nullptr},
        {kX,            {}, nullptr},
        {kI32,          {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kLeftBracket,  {}, nullptr},
        {kI32,          {}, nullptr},
        {kNumber,       44, nullptr},
        {kComma,        {}, nullptr},
        {kI32,          {}, nullptr},
        {kNumber,       45, nullptr},
        {kRightBracket, {}, nullptr},
        {kRightBracket, {}, nullptr},
        {kEOF,          {}, nullptr},
    };

    std::shared_ptr<Constant> expected = std::make_shared<ArrayConstant>(
        std::make_unique<ArrayType>(
            std::make_unique<ArrayType>(
                std::make_unique<I32>(),
                2
            ),
            2
        ),
        std::vector<std::shared_ptr<Constant>>{
            std::make_shared<ArrayConstant>(
                std::make_unique<ArrayType>(
                    std::make_unique<I32>(),
                    2
                ),
                std::vector<std::shared_ptr<Constant>>{
                    std::make_shared<I32Constant>(42),
                    std::make_shared<I32Constant>(43),
                }
            ),
            std::make_shared<ArrayConstant>(
                std::make_unique<ArrayType>(
                    std::make_unique<I32>(),
                    2
                ),
                std::vector<std::shared_ptr<Constant>>{
                    std::make_shared<I32Constant>(44),
                    std::make_shared<I32Constant>(45),
                }
            ),
        }
    );

    EXPECT_EQ(*Parser(input.begin()).parseConstant(*expected->type()), *expected);
}

} // namespace mini_llvm::ir
