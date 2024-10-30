#include <gtest/gtest.h>

#include "../../include/VFS/User.hpp"


TEST(UserTest, TestUserCreationAndGetters) {
  tmn_vfs::User* user1 = new tmn_vfs::User("user1", "User One", "hash1", tmn_vfs::UserStatus::LOCAL);
  tmn_vfs::User* user2 = new tmn_vfs::User("user2", "User Two", "hash2", tmn_vfs::UserStatus::SUPER);
  tmn_vfs::User* user3 = new tmn_vfs::User("user3", "User Three", "hash3", tmn_vfs::UserStatus::GUEST);

  ASSERT_EQ(user1->GetUserName(), "user1");
  ASSERT_EQ(user2->GetUserName(), "user2");
  ASSERT_EQ(user3->GetUserName(), "user3");

  ASSERT_EQ(user1->GetFullName(), "User One");
  ASSERT_EQ(user2->GetFullName(), "User Two");
  ASSERT_EQ(user3->GetFullName(), "User Three");

  ASSERT_EQ(user1->GetPasswordHash(), "hash1");
  ASSERT_EQ(user2->GetPasswordHash(), "hash2");
  ASSERT_EQ(user3->GetPasswordHash(), "hash3");

  ASSERT_EQ(user1->GetStatus(), tmn_vfs::UserStatus::LOCAL);
  ASSERT_EQ(user2->GetStatus(), tmn_vfs::UserStatus::SUPER);
  ASSERT_EQ(user3->GetStatus(), tmn_vfs::UserStatus::GUEST);

  delete user1;
  delete user2;
  delete user3;
}