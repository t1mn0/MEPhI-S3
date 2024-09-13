cmake_minimum_required(VERSION 3.20)

project(mephi-3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Werror

    -Wall
    -Wextra
)

add_library ( exceptions SHARED
    src/Exceptions/Exception.cpp
    src/Exceptions/AllocException.cpp
    src/Exceptions/IOException.cpp
    src/Exceptions/LogicException.cpp
    src/Exceptions/RuntimeException.cpp
)

add_executable( main
    main.cpp
    src/tmn.cpp
) 


# target_link_libraries(main exceptions)


find_package(GTest REQUIRED)

if(GTEST_FOUND)

    set(GTEST_INCLUDE_DIR "/usr/include") 
    set(GTEST_LIBRARY "/usr/lib/libgtest.a") 
    set(GTEST_MAIN_LIBRARY "/usr/lib/libgtest_main.a")


    enable_testing()
    add_executable(func_testing_uniqueptr test/src/FuncTestUniquePtr.cpp)
    add_executable(func_testing_sharedptr test/src/FuncTestSharedPtr.cpp)

    add_executable(load_testing_uniqueptr test/src/LoadTestUniquePtr.cpp)
    add_executable(load_testing_sharedptr test/src/LoadTestSharedPtr.cpp)

    target_link_libraries(func_testing_uniqueptr GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(func_testing_sharedptr GTest::gtest GTest::gtest_main exceptions)

    target_link_libraries(load_testing_uniqueptr GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(load_testing_sharedptr GTest::gtest GTest::gtest_main exceptions)

    add_test(NAME functesting_uniqueptr COMMAND func_testing_uniqueptr)
    add_test(NAME functesting_sharedptr COMMAND func_testing_sharedptr)
    
    add_test(NAME loadtesting_uniqueptr COMMAND load_testing_uniqueptr)
    add_test(NAME loadtesting_sharedptr COMMAND load_testing_sharedptr)

endif()