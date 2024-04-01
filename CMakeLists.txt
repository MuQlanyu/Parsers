cmake_minimum_required(VERSION 3.23)
project(Parser)

set(CMAKE_CXX_STANDARD 20)

# Основное задание
add_subdirectory(src)

add_executable(CYK CYK_main.cpp)
target_link_libraries(CYK parser_lib cyk_lib)

add_executable(Earley Earley_main.cpp)
target_link_libraries(Earley parser_lib earley_lib)

# Для тестов

add_subdirectory(tests)
find_package(GTest REQUIRED)

target_link_libraries(Tests GTest::gtest GTest::gtest_main parser_lib cyk_lib earley_lib)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Testing
enable_testing()

add_test(Test ${CMAKE_CURRENT_BINARY_DIR}/Tests)
