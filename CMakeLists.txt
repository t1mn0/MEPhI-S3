cmake_minimum_required(VERSION 3.20)

project(mephi-3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Werror

    -Wall
    -Wextra
)



find_package(GTest REQUIRED)

if(GTEST_FOUND)

    set(GTEST_INCLUDE_DIR "/usr/include") 
    set(GTEST_LIBRARY "/usr/lib/libgtest.a") 
    set(GTEST_MAIN_LIBRARY "/usr/lib/libgtest_main.a")

    add_executable( mephi-3 
        main.cpp
        src/tmn.cpp
        #test/src/TestUniquePtr.cpp
    ) 



    enable_testing()
    add_executable(testing_uniqueptr test/src/TestUniquePtr.cpp)
    add_executable(testing_sharedptr test/src/TestSharedPtr.cpp)
    target_link_libraries(testing_uniqueptr GTest::gtest GTest::gtest_main)
    target_link_libraries(testing_sharedptr GTest::gtest GTest::gtest_main)
    add_test(NAME testing_uniqueptr COMMAND testing_uniqueptr)
    add_test(NAME testing_sharedptr COMMAND testing_sharedptr)

endif()