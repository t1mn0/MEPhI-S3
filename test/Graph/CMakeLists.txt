cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

set(TESTS
  functional_test_unweighted_graph
)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

foreach(TEST ${TESTS})
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries(${TEST} GTest::gtest GTest::gtest_main Exceptions)
  target_compile_definitions(${TEST} PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
