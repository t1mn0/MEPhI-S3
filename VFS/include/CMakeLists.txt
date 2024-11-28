cmake_minimum_required(VERSION 3.20)
project(VFS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(VFS SHARED
    ../src/FileDescriptor.cpp
    ../src/Group.cpp
    ../src/User.cpp
    ../src/Utils.cpp
    ../src/vfs_init.cpp
    ../src/vfs_save.cpp
    ../src/vfs_users.cpp
    ../src/vfs_groups.cpp
    ../src/vfs_files.cpp
    ../src/VFS.cpp
    ../gui/help.cpp
    ../gui/handling_users.cpp
    ../gui/handling_groups.cpp
    ../gui/handling_files.cpp
    ../gui/View.cpp
)