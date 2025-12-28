// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include "mini-llvm/utils/PCG32.h"

namespace mini_llvm {

TEST(PCG32Test, Determinism) {
    PCG32 gen1(42);
    PCG32 gen2(42);

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(gen1(), gen2());
    }
}

} // namespace mini_llvm
