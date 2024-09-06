cmake_minimum_required(VERSION 3.20)
project(SmartPtr VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SmartPtr STATIC
  UniquePtr/UniquePtr.cpp
  SharedPtr/SharedPtr.cpp
)

install(TARGETS SmartPtr DESTINATION lib)
install(FILES UniquePtr/UniquePtr.h SharedPtr/SharedPtr.h DESTINATION include)