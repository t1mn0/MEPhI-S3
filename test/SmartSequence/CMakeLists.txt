cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

add_executable(test_smart_seq functional_test_smartseq.cpp)
target_link_libraries(test_smart_seq GTest::gtest GTest::gtest_main Exceptions)
add_test(NAME test_smart_seq COMMAND test_smart_seq)