#include <gtest/gtest.h>

#include "../../include/VFS/User.hpp"

TEST(UserTest, Test) {
  tmn_vfs::User* user1 = new tmn_vfs::User("user1", "User One", "hash1", tmn_vfs::UserStatus::LOCAL);
  tmn_vfs::User* user2 = new tmn_vfs::User("user2", "User Two", "hash2", tmn_vfs::UserStatus::SUPER);
  tmn_vfs::User* user3 = new tmn_vfs::User("user3", "User Three", "hash3", tmn_vfs::UserStatus::SYSTEM);

  ASSERT_EQ(user1->UserName(), "user1");
  ASSERT_EQ(user2->UserName(), "user2");
  ASSERT_EQ(user3->UserName(), "user3");

  ASSERT_EQ(user1->FullName(), "User One");
  ASSERT_EQ(user2->FullName(), "User Two");
  ASSERT_EQ(user3->FullName(), "User Three");

  ASSERT_EQ(user1->Status(), tmn_vfs::UserStatus::LOCAL);
  ASSERT_EQ(user2->Status(), tmn_vfs::UserStatus::SUPER);
  ASSERT_EQ(user3->Status(), tmn_vfs::UserStatus::SYSTEM);

  delete user1;
  delete user2;
  delete user3;
}