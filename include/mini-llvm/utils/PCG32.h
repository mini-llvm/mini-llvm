// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <limits>

namespace mini_llvm {

class PCG32 {
public:
    using result_type = uint32_t;

    explicit PCG32(uint64_t seed, uint64_t seq = UINT64_C(0)) noexcept {
        state_ = UINT64_C(0);
        inc_ = (seq << 1) | UINT64_C(1);
        (*this)();
        state_ += seed;
        (*this)();
    }

    uint32_t operator()() noexcept {
        uint64_t oldState = state_;
        state_ = oldState * UINT64_C(6364136223846793005) + inc_;
        uint32_t shifted = static_cast<uint32_t>(((oldState >> 18) ^ oldState) >> 27);
        uint32_t rot = oldState >> 59;
        return (shifted >> rot) | (shifted << ((-rot) & 31));
    }

    static constexpr uint32_t min() noexcept {
        return UINT32_C(0);
    }

    static constexpr uint32_t max() noexcept {
        return std::numeric_limits<uint32_t>::max();
    }

private:
    uint64_t state_;
    uint64_t inc_;
};

} // namespace mini_llvm
