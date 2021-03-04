cmake_minimum_required(VERSION 3.16)
project (hackerrank_cpp)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(main main.cpp)

enable_testing()
find_package(GTest REQUIRED)

add_compile_definitions(TESTS_TIMEOUT_IN_SECONDS=10)

set_source_files_properties(end_to_end_tests.cpp PROPERTIES COMPILE_FLAGS -Wno-gnu-zero-variadic-macro-arguments)
add_executable(end_to_end_tests end_to_end_tests.cpp)
target_link_libraries(end_to_end_tests gtest gtest_main stdc++fs pthread)
add_test(end_to_end_tests end_to_end_tests)
