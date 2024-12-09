cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

add_executable(test_tuple FuncTestTuple.cpp)
target_link_libraries(test_tuple GTest::gtest GTest::gtest_main)
add_test(NAME test_tuple COMMAND test_tuple)