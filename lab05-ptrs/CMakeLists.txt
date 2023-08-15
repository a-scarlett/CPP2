cmake_minimum_required(VERSION 3.22)
project(tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(tests solution/unique_ptr_test.cpp solution/doctest_main.cpp)