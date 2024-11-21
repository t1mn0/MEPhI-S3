cmake_minimum_required(VERSION 3.20)
project(ExceptionsLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Exceptions SHARED
    ../../src/Exceptions/Exception.cpp
    ../../src/Exceptions/AllocException.cpp
    ../../src/Exceptions/IOException.cpp
    ../../src/Exceptions/LogicException.cpp
    ../../src/Exceptions/RuntimeException.cpp
)