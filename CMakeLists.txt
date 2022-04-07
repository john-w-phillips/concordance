project(CodeChallenge)
include(CTest)
cmake_minimum_required(VERSION 3.18)
find_package(GTest REQUIRED)
add_library(code_challenge STATIC)
add_subdirectory(src)
include_directories(include)
add_subdirectory(tests)



