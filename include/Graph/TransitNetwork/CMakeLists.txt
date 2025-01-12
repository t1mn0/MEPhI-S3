cmake_minimum_required(VERSION 3.20)
project(TransitNetwork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TransitNetwork STATIC
    UnitClasses.cpp
    TransitNetwork.cpp
)