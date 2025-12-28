// SPDX-License-Identifier: MIT

#include <vector>

#include <gtest/gtest.h>

#include "mini-llvm/utils/SequenceOps.h"

using namespace mini_llvm::sequence_ops;

namespace mini_llvm {

TEST(SequenceOpsTest, Append) {
    std::vector<int> actual{1, 2, 3};
    actual += 4, 5, 6;
    std::vector<int> expected{1, 2, 3, 4, 5, 6};
    EXPECT_EQ(actual, expected);
}

TEST(SequenceOpsTest, Concat) {
    std::vector<int> lhs{1, 2, 3};
    std::vector<int> actual = lhs + 4 + 5 + 6;
    std::vector<int> expected{1, 2, 3, 4, 5, 6};
    EXPECT_EQ(actual, expected);
}

} // namespace mini_llvm
