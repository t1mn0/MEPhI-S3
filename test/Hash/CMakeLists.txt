cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

set(TESTS
  test_hash
  functional_test_hashtable
  functional_test_hashset
  load_test_hashtable
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

find_package(Gnuplot REQUIRED)
if (NOT Gnuplot_FOUND)
  message(FATAL_ERROR "Gnuplot not found. Please install the required dependencies for this project to work")
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS iostreams)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found. Please install the required dependencies for this project to work")
endif()

add_executable(parse_testdata parse_testdata.cpp)
add_executable(plot_data plot_data.cpp)
target_compile_definitions(parse_testdata PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(plot_data PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(plot_data ${GNUPlot_LIBRARIES} ${Boost_LIBRARIES} Exceptions)