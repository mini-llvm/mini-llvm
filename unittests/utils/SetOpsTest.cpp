// SPDX-License-Identifier: MIT

#include <unordered_set>

#include <gtest/gtest.h>

#include "mini-llvm/utils/SetOps.h"

using namespace mini_llvm::set_ops;

struct TestCase {
    std::unordered_set<int> lhs;
    std::unordered_set<int> rhs;
    std::unordered_set<int> expected;
};

namespace mini_llvm {

TEST(SetOpsTest, Union) {
    TestCase testCases[] = {
        {{}, {}, {}},
        {{1}, {}, {1}},
        {{}, {1}, {1}},
        {{1}, {1}, {1}},
        {{1, 2, 3}, {2, 3, 4}, {1, 2, 3, 4}},
    };

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs | rhs;
        EXPECT_EQ(actual, expected);
    }

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs | std::unordered_set(rhs);
        EXPECT_EQ(actual, expected);
    }

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs;
        actual |= rhs;
        EXPECT_EQ(actual, expected);
    }

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs;
        actual |= std::unordered_set(rhs);
        EXPECT_EQ(actual, expected);
    }
}

TEST(SetOpsTest, Intersection) {
    TestCase testCases[] = {
        {{}, {}, {}},
        {{1}, {}, {}},
        {{}, {1}, {}},
        {{1}, {1}, {1}},
        {{1, 2, 3}, {2, 3, 4}, {2, 3}},
    };

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs & rhs;
        EXPECT_EQ(actual, expected);
    }

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs;
        actual &= rhs;
        EXPECT_EQ(actual, expected);
    }
}

TEST(SetOpsTest, Difference) {
    TestCase testCases[] = {
        {{}, {}, {}},
        {{1}, {}, {1}},
        {{}, {1}, {}},
        {{1}, {1}, {}},
        {{1, 2, 3}, {2, 3, 4}, {1}},
    };

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs - rhs;
        EXPECT_EQ(actual, expected);
    }

    for (const auto &[lhs, rhs, expected] : testCases) {
        std::unordered_set<int> actual = lhs;
        actual -= rhs;
        EXPECT_EQ(actual, expected);
    }
}

} // namespace mini_llvm
