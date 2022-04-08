project(CodeChallenge)
include(CTest)
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.18)
add_library(code_challenge STATIC)
target_compile_features(code_challenge PRIVATE cxx_std_17)
include_directories(include)
add_subdirectory(src)
if (BUILD_UNIT_TESTS)
  find_package(GTest REQUIRED)  
  add_subdirectory(tests)
endif()



