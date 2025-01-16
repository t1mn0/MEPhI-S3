cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

target_include_directories(Exceptions PUBLIC ../include/Exceptions)

add_executable(ftest_sortalgs functional_test_sortalgs.cpp)
target_compile_definitions(ftest_sortalgs PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(ftest_sortalgs GTest::gtest GTest::gtest_main Exceptions)
add_test(NAME ftest_sortalgs COMMAND ftest_sortalgs)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS iostreams)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found. Please install the required dependencies for this project to work")
endif()

add_executable(plot plot.cpp)
target_compile_definitions(plot PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(plot ${GNUPlot_LIBRARIES} ${Boost_LIBRARIES} Exceptions)