cmake_minimum_required(VERSION 3.20)

project(mini-llvm VERSION 0.1.0 LANGUAGES CXX)

option(MINI_LLVM_TESTS "Build tests." ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        add_compile_definitions(_GLIBCXX_ASSERTIONS)
    endif()

    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(lib)
add_subdirectory(tools)

if(MINI_LLVM_TESTS)
    enable_testing()
    add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL SYSTEM)
    add_subdirectory(tests)
endif()
