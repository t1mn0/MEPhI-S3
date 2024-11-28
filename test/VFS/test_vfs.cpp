#include <gtest/gtest.h>

#include <filesystem>

#include "../../include/VFS/VFS.hpp"

TEST(VirtualFileSystemTest, InitVFS) {
    auto vfs = tmn_vfs::VirtualFileSystem::Init();
    
}