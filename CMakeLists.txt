cmake_minimum_required(VERSION 3.20)

project(mini-llvm VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(tools)

add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL SYSTEM)

enable_testing()

add_subdirectory(tests)
