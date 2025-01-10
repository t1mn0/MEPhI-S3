cmake_minimum_required(VERSION 3.20)
project(TransitNetwork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Exceptions PUBLIC ../../Exceptions)

add_library(TransitNetwork STATIC
    UnitClasses.cpp
)

#add_executable(main main.cpp)
#target_link_libraries(main TransitNetwork Exceptions)