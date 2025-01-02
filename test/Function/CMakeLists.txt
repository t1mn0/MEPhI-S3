cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

add_executable(test_function functional_test_function.cpp)
target_link_libraries(test_function GTest::gtest GTest::gtest_main Exceptions)
add_test(NAME test_function COMMAND test_function)