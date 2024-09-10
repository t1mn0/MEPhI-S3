cmake_minimum_required(VERSION 3.10)

project(mephi-3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

set(GTEST_INCLUDE_DIR "/usr/include") 
set(GTEST_LIBRARY "/usr/lib/libgtest.a") 
set(GTEST_MAIN_LIBRARY "/usr/lib/libgtest_main.a")


add_compile_options(
    -Werror

    -Wall
    -Wextra
)



add_executable( mephi-3 
main.cpp
src/tmn.cpp
#test/src/TestUniquePtr.cpp
) 



enable_testing()
add_executable(testing test/src/TestUniquePtr.cpp)
target_link_libraries(testing GTest::gtest GTest::gtest_main)
add_test(NAME testing COMMAND testing)