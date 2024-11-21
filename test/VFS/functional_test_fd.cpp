#include <gtest/gtest.h>

#include "../../include/VFS/FileDescriptor.hpp"

TEST(FileDescriptorTest, Constructors) {
    tmn_vfs::FileDescriptor file_info_1(false, "/path", "/VFS/dir1/file", "testuser1", "testgroup1");
    tmn_vfs::FileDescriptor file_info_2(true, "/path", "/VFS/dir2/file", "testuser2");
    tmn_vfs::FileDescriptor file_info_3(false, "/path", "/VFS/dir3/file", "testuser3", 
        tmn_vfs::FilePermissions(tmn_vfs::Permission::NONE, tmn_vfs::Permission::NONE, tmn_vfs::Permission::NONE));
    tmn_vfs::FileDescriptor file_info_4(false, "/path", "/VFS/dir4/file", "testuser4", "testgroup4",
        tmn_vfs::FilePermissions(tmn_vfs::Permission::READWRITE, tmn_vfs::Permission::READWRITE, tmn_vfs::Permission::READWRITE));
    
    ASSERT_FALSE(file_info_1.IsDirectory());
    ASSERT_TRUE(file_info_2.IsDirectory());
    ASSERT_FALSE(file_info_3.IsDirectory());
    ASSERT_FALSE(file_info_4.IsDirectory());

    ASSERT_EQ(file_info_1.VirtualPath(), "/VFS/dir1/file");
    ASSERT_EQ(file_info_2.VirtualPath(), "/VFS/dir2/file");
    ASSERT_EQ(file_info_3.VirtualPath(), "/VFS/dir3/file");
    ASSERT_EQ(file_info_4.VirtualPath(), "/VFS/dir4/file");

    ASSERT_EQ(file_info_1.PhysicalPath(), "/path");
    ASSERT_EQ(file_info_2.PhysicalPath(), "/path");
    ASSERT_EQ(file_info_3.PhysicalPath(), "/path");
    ASSERT_EQ(file_info_4.PhysicalPath(), "/path");

    ASSERT_EQ(file_info_1.Size(), 0);
    ASSERT_EQ(file_info_2.Size(), 0);
    ASSERT_EQ(file_info_3.Size(), 0);
    ASSERT_EQ(file_info_4.Size(), 0);

    ASSERT_EQ(file_info_1.OwnerUser(), "testuser1");
    ASSERT_EQ(file_info_2.OwnerUser(), "testuser2");
    ASSERT_EQ(file_info_3.OwnerUser(), "testuser3");
    ASSERT_EQ(file_info_4.OwnerUser(), "testuser4");

    ASSERT_EQ(file_info_1.OwnerGroup(), "testgroup1");
    ASSERT_EQ(file_info_2.OwnerGroup(), "testuser2");
    ASSERT_EQ(file_info_3.OwnerGroup(), "testuser3");
    ASSERT_EQ(file_info_4.OwnerGroup(), "testgroup4");

    ASSERT_EQ(file_info_1.WhenCreated(), file_info_2.WhenModified());
}