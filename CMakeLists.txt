cmake_minimum_required(VERSION 3.20)

project(mini-llvm VERSION 0.1.0 LANGUAGES CXX)

option(MINI_LLVM_TESTS "Build tests." ON)

add_subdirectory(lib)
add_subdirectory(tools)

if(MINI_LLVM_TESTS)
    enable_testing()
    add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL SYSTEM)
    add_subdirectory(tests)
endif()
