cmake_minimum_required(VERSION 3.20)
project(VFS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOME_DIR "$ENV{HOME}") 
set(VFS_DIR "${HOME_DIR}/.config/vfs")

if(NOT EXISTS "${VFS_DIR}")
  execute_process(COMMAND mkdir -p "${VFS_DIR}" RESULT_VARIABLE CREATE_DIR_RESULT)

  if(CREATE_DIR_RESULT EQUAL 0)
    message(STATUS "'${VFS_DIR}' directory with user-config was successfully created")
  else()
    message(FATAL_ERROR "Error while creating directory '${VFS_DIR}'. Error code: ${CREATE_DIR_RESULT}")
  endif()

else()
  message(STATUS "'${VFS_DIR}' directory already exists. All good")

endif()

if(EXISTS "${VFS_DIR}")
  set(ENTRY_POINT_PATH "${VFS_DIR}")
  set(USERS_PATH "${VFS_DIR}/users.dat")
  set(GROUPS_PATH "${VFS_DIR}/groups.dat")
  set(FILE_DESCRIPTORS_PATH "${VFS_DIR}/fd.dat")
  set(VFS_CONFIG_PATH "${VFS_DIR}/config.dat")

  add_definitions(
    -DENTRY_POINT_PATH="${ENTRY_POINT_PATH}"
    -DUSERS_PATH="${USERS_PATH}"
    -DGROUPS_PATH="${GROUPS_PATH}"
    -DFILE_DESCRIPTORS_PATH="${FILE_DESCRIPTORS_PATH}"
    -DVFS_CONFIG_PATH="${VFS_CONFIG_PATH}"
  )
endif()

add_library(VFS STATIC
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