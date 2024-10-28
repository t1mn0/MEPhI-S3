cmake_minimum_required(VERSION 3.20)
project(S3 VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(exceptions SHARED
    src/Exceptions/Exception.cpp
    src/Exceptions/AllocException.cpp
    src/Exceptions/IOException.cpp
    src/Exceptions/LogicException.cpp
    src/Exceptions/RuntimeException.cpp
)

include_directories(include/VFS)

#find_package(Curses REQUIRED)
#if(CURSES_FOUND)
#    add_executable(gui_l1 gui/L1/main.cpp)
#    target_link_libraries(gui_l1 ${CURSES_LIBRARY} exceptions)
#endif()

add_library(VFS SHARED
    src/VFS/FileInfo.cpp
    src/VFS/Group.cpp
    src/VFS/User.cpp
    src/VFS/Utils.cpp
    src/VFS/VFS.cpp
)

find_package(GTest REQUIRED)
if(GTEST_FOUND)

    set(GTEST_INCLUDE_DIR "/usr/include")
    set(GTEST_LIBRARY "/usr/lib/libgtest.a")
    set(GTEST_MAIN_LIBRARY "/usr/lib/libgtest_main.a")

    enable_testing()
    # Тесты к ЛР-1:
    #add_executable(func_test_uniqueptr test/SmartPtr/FuncTestUniquePtr.cpp)
    #add_executable(func_test_sharedptr test/SmartPtr/FuncTestSharedPtr.cpp)
    #add_executable(load_test_uniqueptr test/SmartPtr/LoadTestUniquePtr.cpp)

    #target_link_libraries(func_test_uniqueptr GTest::gtest GTest::gtest_main exceptions)
    #target_link_libraries(func_test_sharedptr GTest::gtest GTest::gtest_main exceptions)
    #target_link_libraries(load_test_uniqueptr GTest::gtest GTest::gtest_main exceptions)

    #add_test(NAME func_test_uniqueptr COMMAND func_test_uniqueptr)
    #add_test(NAME func_test_sharedptr COMMAND func_test_sharedptr)
    #add_test(NAME load_test_uniqueptr COMMAND load_test_uniqueptr)

    # Тесты к ЛР-3:
    add_executable(func_test_hashtable test/HashTable/FuncTestHashTable.cpp)
    add_executable(func_test_hashset test/HashSet/FuncTestHashSet.cpp)
    add_executable(load_test_hashtable test/HashTable/LoadTestHashTable.cpp)
    add_executable(load_test_hashset test/HashSet/LoadTestHashSet.cpp)
    add_executable(func_test_user test/VFS/UserTest.cpp)
    add_executable(func_test_group test/VFS/GroupTest.cpp)
    add_executable(func_test_fileinfo test/VFS/FileInfoTest.cpp)

    target_link_libraries(func_test_hashtable GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(func_test_hashset GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(load_test_hashtable GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(load_test_hashset GTest::gtest GTest::gtest_main exceptions)
    target_link_libraries(func_test_user GTest::gtest GTest::gtest_main VFS)
    target_link_libraries(func_test_group GTest::gtest GTest::gtest_main VFS)
    target_link_libraries(func_test_fileinfo GTest::gtest GTest::gtest_main VFS)

    add_test(NAME func_test_hashtable COMMAND func_test_hashtable)
    add_test(NAME func_test_hashset COMMAND func_test_hashset)
    add_test(NAME load_test_hashtable COMMAND load_test_hashtable)
    add_test(NAME load_test_hashset COMMAND load_test_hashset)
    add_test(NAME func_test_user COMMAND func_test_user)
    add_test(NAME func_test_group COMMAND func_test_group)
    add_test(NAME func_test_fileinfo COMMAND func_test_fileinfo)

endif()
