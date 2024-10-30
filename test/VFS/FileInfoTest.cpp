#include <gtest/gtest.h>

#include "../../include/VFS/FileInfo.hpp"

TEST(FileInfoTest, Constructor) {
    tmn_vfs::FileInfo file_info(false, "/path", "/VFS/dir1/file", "testuser");
    ASSERT_FALSE(file_info.IsDirectory());
    ASSERT_EQ(file_info.GetVirtualPath(), "/VFS/dir1/file");
    ASSERT_EQ(file_info.GetSize(), 0);
    ASSERT_EQ(file_info.GetCreatorName(), "testuser");
    ASSERT_EQ(file_info.GetCreationTime(), file_info.GetModificationTime());
}

TEST(FileInfoTest, SetModificationTime) {
    tmn_vfs::FileInfo file_info(false, "/path", "/VFS/dir1/file", "testuser");
    std::string new_time = "11-12-13-04-12-2007";
    file_info.SetModificationTime(new_time);
    ASSERT_EQ(file_info.GetModificationTime(), new_time);
}

TEST(FileInfoTest, AddInnerFile) {
    tmn_vfs::FileInfo file_info(true, "/path", "/VFS/dir1/file", "testuser");
    file_info.AddInnerFile("file1.txt");
    file_info.AddInnerFile("file2.txt");
    file_info.AddInnerFile("file3.txt");
    ASSERT_EQ(file_info.GetSize(), 3);
}

TEST(FileInfoTest, CanAccess) {
    tmn_vfs::FileInfo file_info(false, "/path", "/VFS/dir1/file", "testuser");
    file_info.AddGroup("developers", tmn_vfs::Permissions::READ | tmn_vfs::Permissions::WRITE);
    file_info.AddGroup("default", tmn_vfs::Permissions::READ);

    ASSERT_FALSE(file_info.CanAccess("developers", tmn_vfs::Permissions::READ | tmn_vfs::Permissions::MOVE));
    ASSERT_FALSE(file_info.CanAccess("developers", tmn_vfs::Permissions::DELETE));
    ASSERT_TRUE(file_info.CanAccess("developers", tmn_vfs::Permissions::READ));
    ASSERT_TRUE(file_info.CanAccess("developers", tmn_vfs::Permissions::WRITE));
    ASSERT_TRUE(file_info.CanAccess("default", tmn_vfs::Permissions::READ));
    ASSERT_FALSE(file_info.CanAccess("default", tmn_vfs::Permissions::WRITE));
    ASSERT_FALSE(file_info.CanAccess("DEFAULT", tmn_vfs::Permissions::MOVE));
    ASSERT_FALSE(file_info.CanAccess("LINUX", tmn_vfs::Permissions::DELETE));
}

TEST(FileInfoTest, Getters){
    tmn_vfs::FileInfo file_info(false, "/path", "/VFS/dir1/file", "testuser");
    ASSERT_EQ(file_info.GetVirtualPath(),"/VFS/dir1/file");
    ASSERT_EQ(file_info.GetCreatorName(), "testuser");
    ASSERT_FALSE(file_info.IsDirectory());
}