cmake_minimum_required(VERSION 3.19)
project (hackerrank_cpp)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-gnu-zero-variadic-macro-arguments)

add_executable(main main.cpp)

enable_testing()
find_package(GTest REQUIRED)

add_executable(end_to_end_tests end_to_end_tests.cpp)
target_link_libraries(end_to_end_tests gtest gtest_main stdc++fs pthread)
add_test(end_to_end_tests end_to_end_tests)

file(COPY test_cases DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
