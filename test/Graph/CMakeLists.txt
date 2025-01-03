cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

set(TESTS
  functional_test_unweighted_graph
  functional_test_singleweighted_graph
  load_test_bfs_singleweighted_graph
  load_test_dfs_singleweighted_graph
  load_test_coloring_singleweighted_graph
  load_test_bfs_unweighted_graph
  load_test_dfs_unweighted_graph
  load_test_coloring_unweighted_graph
)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

add_library(graph_test_utils STATIC graph_test_utils.cpp)

foreach(TEST ${TESTS})
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries(${TEST} GTest::gtest GTest::gtest_main Exceptions graph_test_utils)
  target_compile_definitions(${TEST} PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS iostreams)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found. Please install the required dependencies for this project to work")
endif()

add_executable(plot_graph_data plot_graph_data.cpp)
target_compile_definitions(parse_testdata PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(plot_graph_data PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(plot_graph_data ${GNUPlot_LIBRARIES} ${Boost_LIBRARIES} Exceptions)