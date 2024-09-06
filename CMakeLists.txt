cmake_minimum_required(VERSION 3.20)

project(mephi-3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Werror

    -Wall
    -Wextra
)

add_subdirectory(src/SmartPtr)
include_directories(src/SmartPtr)



add_executable(main  main.cpp)
target_link_libraries(main  SmartPtr) 



find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})

enable_testing()
add_executable(my_tests my_test.cpp)
target_link_libraries(my_tests gtest gtest_main)
add_test(NAME my_tests COMMAND my_tests) 