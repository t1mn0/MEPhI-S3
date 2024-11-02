cmake_minimum_required(VERSION 3.20)
project(SEMESTER-3 VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Exceptions SHARED
    src/Exceptions/Exception.cpp
    src/Exceptions/AllocException.cpp
    src/Exceptions/IOException.cpp
    src/Exceptions/LogicException.cpp
    src/Exceptions/RuntimeException.cpp
)

add_library(VFS SHARED
    src/VFS/FileInfo.cpp
    src/VFS/Group.cpp
    src/VFS/User.cpp
    src/VFS/Utils.cpp
    src/VFS/VFS.cpp
)

include_directories(include include/VFS include/Exceptions)


find_package(GTest REQUIRED)
if(GTEST_FOUND)
    enable_testing()

    add_executable(func_test_hashtable test/HashTable/FuncTestHashTable.cpp)
    add_executable(func_test_hashset test/HashSet/FuncTestHashSet.cpp)
    add_executable(load_test_hashtable test/HashTable/LoadTestHashTable.cpp)
    add_executable(load_test_hashset test/HashSet/LoadTestHashSet.cpp)
    add_executable(func_test_user test/VFS/UserTest.cpp)
    add_executable(func_test_group test/VFS/GroupTest.cpp)
    add_executable(func_test_fileinfo test/VFS/FileInfoTest.cpp)
    add_executable(load_test_indextable test/VFS/LoadTestIndexTable.cpp)
    add_executable(func_test_vfs test/VFS/VFSTest.cpp)

    target_link_libraries(func_test_hashtable GTest::gtest GTest::gtest_main Exceptions)
    target_link_libraries(func_test_hashset GTest::gtest GTest::gtest_main Exceptions)
    target_link_libraries(load_test_hashtable GTest::gtest GTest::gtest_main Exceptions)
    target_link_libraries(load_test_hashset GTest::gtest GTest::gtest_main Exceptions)
    target_link_libraries(func_test_user GTest::gtest GTest::gtest_main VFS Exceptions)
    target_link_libraries(func_test_group GTest::gtest GTest::gtest_main VFS Exceptions)
    target_link_libraries(func_test_fileinfo GTest::gtest GTest::gtest_main VFS Exceptions)
    target_link_libraries(load_test_indextable GTest::gtest GTest::gtest_main VFS Exceptions)
    target_link_libraries(func_test_vfs GTest::gtest GTest::gtest_main VFS Exceptions)

    add_test(NAME func_test_hashtable COMMAND func_test_hashtable)
    add_test(NAME func_test_hashset COMMAND func_test_hashset)
    add_test(NAME load_test_hashtable COMMAND load_test_hashtable)
    add_test(NAME load_test_hashset COMMAND load_test_hashset)
    add_test(NAME func_test_user COMMAND func_test_user)
    add_test(NAME func_test_group COMMAND func_test_group)
    add_test(NAME func_test_fileinfo COMMAND func_test_fileinfo)
    add_test(NAME load_test_indextable COMMAND load_test_indextable)
    add_test(NAME func_test_vfs COMMAND func_test_vfs)

endif()
