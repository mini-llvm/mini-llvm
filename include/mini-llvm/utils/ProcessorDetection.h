// SPDX-License-Identifier: MIT

#pragma once

#if defined(__x86_64__) || defined(_M_X64)
#   define MINI_LLVM_X86_64
#elif defined(__i386__) || defined(_M_IX86)
#   define MINI_LLVM_I386
#elif defined(__aarch64__) || defined(_M_ARM64)
#   define MINI_LLVM_AARCH64
#elif defined(__arm__) || defined(_M_ARM)
#   define MINI_LLVM_ARM
#elif defined(__powerpc64__) || defined(__ppc64__)
#   define MINI_LLVM_PPC64
#elif defined(__powerpc__) || defined(__ppc__)
#   define MINI_LLVM_PPC
#elif defined(__riscv) && __riscv_xlen == 64
#   define MINI_LLVM_RISCV64
#elif defined(__riscv) && __riscv_xlen == 32
#   define MINI_LLVM_RISCV32
#endif
