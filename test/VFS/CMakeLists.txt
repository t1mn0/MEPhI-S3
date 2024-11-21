cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../include/Exceptions)
target_include_directories(VFS PUBLIC ../include/VFS)

set(UNIT_TESTS
  functional_test_user
  functional_test_group
  functional_test_fd
)


foreach(UNIT_TEST ${UNIT_TESTS})
  add_executable(${UNIT_TEST} ${UNIT_TEST}.cpp)
  target_link_libraries(${UNIT_TEST} GTest::gtest GTest::gtest_main VFS Exceptions)
  add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})
endforeach()

add_executable(test_direct_search test_direct_search.cpp utils_for_tests.cpp)
add_executable(test_indirect_search test_indirect_search.cpp utils_for_tests.cpp)
target_link_libraries(test_direct_search GTest::gtest GTest::gtest_main VFS Exceptions)
target_link_libraries(test_indirect_search GTest::gtest GTest::gtest_main VFS Exceptions)
add_test(NAME test_direct_search COMMAND test_direct_search)
add_test(NAME test_indirect_search COMMAND test_indirect_search)