cmake_minimum_required(VERSION 3.20)
project(SEMESTER-3 VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Include Directories & Libs ---
set(INCLUDE_DIRS include include/Exceptions)

add_subdirectory(include/Exceptions)

target_include_directories(Exceptions INTERFACE ${INCLUDE_DIRS})

# --- Tests (GTest) ---
find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
    return()
else()
    option(BUILD_TESTS "Build tests" ON)

    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(test/Tuple)
        add_subdirectory(test/Graph)
    endif()
endif()