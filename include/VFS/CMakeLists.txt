cmake_minimum_required(VERSION 3.20)
project(VFS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(VFS SHARED
    ../../src/VFS/FileDescriptor.cpp
    ../../src/VFS/Group.cpp
    ../../src/VFS/User.cpp
    ../../src/VFS/Utils.cpp
    ../../src/VFS/VFS.cpp
)