cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../include/Exceptions)
target_include_directories(VFS PUBLIC ../include/VFS)

set(TESTS
  test_hash
  functional_test_hashtable
  functional_test_hashset
  load_test_hashtable
  load_test_hashset
)


foreach(TEST ${TESTS})
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries(${TEST} GTest::gtest GTest::gtest_main Exceptions)
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()