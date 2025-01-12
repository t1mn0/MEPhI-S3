cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

target_include_directories(Exceptions PUBLIC ../include/Exceptions)
target_include_directories(TransitNetwork PUBLIC ../include/Graph/TransitNetwork)

add_executable(test_transitnetwork functional_test_transitnetwork.cpp)
target_link_libraries(test_transitnetwork GTest::gtest GTest::gtest_main TransitNetwork Exceptions)
add_test(NAME test_transitnetwork COMMAND test_transitnetwork)