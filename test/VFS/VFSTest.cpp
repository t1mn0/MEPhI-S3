#include <gtest/gtest.h>

#include <filesystem>

#include "../../include/VFS/VFS.hpp"
#include "../../include/VFS/User.hpp"

TEST(VirtualFileSystemTest, InitVFS) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);
    
    ASSERT_EQ(vfs.CountGroup(), 1);
    ASSERT_EQ(vfs.CountUser(), 1);

    ASSERT_EQ(vfs.WhoUser(), "timno");
    ASSERT_EQ(vfs.PWD(), "files");

    std::filesystem::path vfs_path(tmn_vfs::MOUNT_DIR + "/" + tmn_vfs::VFS_DIR);

    ASSERT_TRUE(std::filesystem::exists(vfs_path / "files"));
    ASSERT_TRUE(std::filesystem::is_directory(vfs_path / "files"));

    for (size_t i = 0; i < tmn_vfs::FILE_COUNT; ++i) {
        std::filesystem::path file_path = vfs_path / "files" / ("File-" + std::to_string(i) + ".txt");
        ASSERT_TRUE(std::filesystem::exists(file_path));
        ASSERT_TRUE(std::filesystem::is_regular_file(file_path));
    }
}

TEST(VirtualFileSystemTest, AddUser) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);

    tmn_vfs::User user1("user-2143", "", "9393939", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user2("admin", "Konstantin", "21233322", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user3("user-3331", "Jostar", "3142443", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user4("user-3331", "Bsah", "5525252", tmn_vfs::UserStatus::SUPER);

    vfs.AddUser(user1);
    vfs.AddUser(user2);
    vfs.AddUser(user3);

    ASSERT_EQ(vfs.CountUser(), 4);
    ASSERT_EQ(vfs.WhoUser(), "timno");
    ASSERT_FALSE(vfs.IsUserInSystem("user-214")); // not in system 
    ASSERT_TRUE(vfs.IsUserInSystem("user-2143")); // in system 
    ASSERT_TRUE(vfs.IsUserInSystem("timno")); // in system 

    vfs.AddUser(user3); // already in system
    ASSERT_EQ(vfs.CountUser(), 4);

    vfs.AddUser(user4); // wrong name, username already in system 
    ASSERT_EQ(vfs.CountUser(), 4);
}

TEST(VirtualFileSystemTest, Authorization) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);

    tmn_vfs::User user1("user-2143", "", "9393939", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user2("admin", "Konstantin", "21233322", tmn_vfs::UserStatus::LOCAL);

    vfs.AddUser(user1);
    vfs.AddUser(user2);

    vfs.Authorization("admin", "3122123"); // user in system, but wrong password
    
    ASSERT_EQ(vfs.WhoUser(), "timno");

    vfs.Authorization("-USER-", "314443"); // user is not in system

    ASSERT_EQ(vfs.WhoUser(), "timno");
    
    vfs.Authorization("user-2143", "9393939"); // all good

    ASSERT_EQ(vfs.WhoUser(), "user-2143");
}

TEST(VirtualFileSystemTest, RemoveUser) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);

    tmn_vfs::User user1("user-2143", "", "9393939", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user2("admin", "Konstantin", "21233322", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user3("user-3331", "Jostar", "3142443", tmn_vfs::UserStatus::GUEST);

    vfs.AddUser(user1);
    vfs.AddUser(user2);
    vfs.AddUser(user3);

    vfs.RemoveUser("user"); // not in system 
    vfs.RemoveUser("user-3333"); // not in system 

    ASSERT_EQ(vfs.CountUser(), 4);

    vfs.Authorization("user-3331", "3142443");

    vfs.RemoveUser("admin"); // in system 
    vfs.RemoveUser("user-3331"); // active user

    ASSERT_EQ(vfs.CountUser(), 3);

    vfs.Authorization("admin", "21233322"); // already not in system 

    ASSERT_EQ(vfs.WhoUser(), "user-3331");

    vfs.RemoveUser("user-2143"); // in system 
    vfs.RemoveUser("timno"); // in system 

    ASSERT_EQ(vfs.CountUser(), 1);
}

TEST(VirtualFileSystemTest, AddGroup) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);

    tmn_vfs::Group groupA("Developers", "timofey", "11110101");
    tmn_vfs::Group groupB("Dev-#1234", "timofey", "1111100000");
    tmn_vfs::Group groupC("Developers", "timofey", "dh823");

    vfs.AddGroup(groupA);
    vfs.AddGroup(groupB);

    ASSERT_EQ(vfs.CountGroup(), 3);

    vfs.AddGroup(groupC);

    ASSERT_EQ(vfs.CountGroup(), 3);

    ASSERT_TRUE(vfs.IsGroupInSystem("Developers"));
    ASSERT_TRUE(vfs.IsGroupInSystem("Dev-#1234"));
    ASSERT_TRUE(vfs.IsGroupInSystem("default"));
}    

TEST(VirtualFileSystemTest, AddToGroup) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user1("user-2143", "", "9393939", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user2("admin", "Konstantin", "21233322", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user3("user-3331", "Jostar", "3142443", tmn_vfs::UserStatus::GUEST);
    
    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);
    
    vfs.AddUser(user1);
    vfs.AddUser(user2);
    vfs.AddUser(user3);

    tmn_vfs::Group groupA("Developers", "timofey", "11110101");
    tmn_vfs::Group groupB("Dev-#1234", "timofey", "1111100000");

    vfs.AddGroup(groupA);
    vfs.AddGroup(groupB);

    vfs.AddToGroup("admin", "Dev-#1234", "11234"); // good username, good groupname, wrong password
    vfs.AddToGroup("admin", "Dev-#0", "1111100000"); // good username, wrong groupname, good password
    vfs.AddToGroup("LOCALUSER", "Dev-#1234", "1111100000"); // wrong username, good groupname, good password
    vfs.AddToGroup("admin", "Dev-#1234", "1111100000"); // good username, good groupname, good password

    ASSERT_FALSE(vfs.IsMember("Dev-#1234", "timno"));
    ASSERT_TRUE(vfs.IsMember("Dev-#1234", "admin"));
}  

TEST(VirtualFileSystemTest, RemoveFromGroup) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user1("user-2143", "", "9393939", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user2("admin", "Konstantin", "21233322", tmn_vfs::UserStatus::LOCAL);
    tmn_vfs::User user3("user-3331", "Jostar", "3142443", tmn_vfs::UserStatus::GUEST);
    tmn_vfs::User user4("user-3113", "Joseph", "5678901", tmn_vfs::UserStatus::GUEST);
    
    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);
    
    vfs.AddUser(user1);
    vfs.AddUser(user2);
    vfs.AddUser(user3);
    vfs.AddUser(user4);

    tmn_vfs::Group groupA("Developers", "timofey", "11110101");

    vfs.AddGroup(groupA);

    vfs.AddToGroup("admin", "Developers", "11110101");
    vfs.AddToGroup("user-3331", "Developers", "11110101");
    vfs.AddToGroup("user-3113", "Developers", "11110101");
    
    ASSERT_EQ(vfs.CountMembers("Developers"), 3);
    ASSERT_EQ(vfs.CountMembers("default"), 5);

    vfs.RemoveFromGroup("admin", "default", "11110101"); // default group! -> unable
    vfs.RemoveFromGroup("fake user", "Developers", "11110101"); // wrong username 
    vfs.RemoveFromGroup("user-3113", "Developers", "123"); // wrong password 
    vfs.RemoveFromGroup("admin", "Developers", "11110101"); // all good 

    ASSERT_EQ(vfs.CountMembers("Developers"), 2);
    ASSERT_EQ(vfs.CountMembers("default"), 5);
} 

TEST(VirtualFileSystemTest, RemoveGroup) {
    tmn_vfs::User creater("timno", "timofey", "1231231239", tmn_vfs::UserStatus::LOCAL);

    auto vfs = tmn_vfs::VirtualFileSystem::Init(creater);

    tmn_vfs::Group groupA("Developers", "timofey", "11110101");
    tmn_vfs::Group groupB("Dev-#1234", "timofey", "1111100000");
    tmn_vfs::Group groupC("Developers", "timofey", "dh823");

    vfs.AddGroup(groupA);
    vfs.AddGroup(groupB);
    vfs.AddGroup(groupC);

    ASSERT_EQ(vfs.CountGroup(), 3);
    
    vfs.RemoveGroup("not in system");

    ASSERT_EQ(vfs.CountGroup(), 3);

    vfs.RemoveGroup("Developers");

    ASSERT_EQ(vfs.CountGroup(), 2);

    vfs.RemoveGroup("default");

    ASSERT_EQ(vfs.CountGroup(), 2);
}   
