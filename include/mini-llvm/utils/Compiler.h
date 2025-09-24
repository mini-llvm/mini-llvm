// SPDX-License-Identifier: MIT

#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(MINI_LLVM_EXPORTS)
        #define MINI_LLVM_EXPORT __declspec(dllexport)
    #elif defined(MINI_LLVM_SHARED)
        #define MINI_LLVM_EXPORT __declspec(dllimport)
    #else
        #define MINI_LLVM_EXPORT
    #endif
#else
    #if defined(MINI_LLVM_EXPORTS)
        #define MINI_LLVM_EXPORT __attribute__((visibility("default")))
    #else
        #define MINI_LLVM_EXPORT
    #endif
#endif
