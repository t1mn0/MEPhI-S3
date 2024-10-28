#include <gtest/gtest.h>

#include "../../include/VFS/User.hpp"

TEST(UserTest, BaseTest) {
  std::string username1 = "python-developer OLEG";
  std::string fullname1 = "Oleg Petrov";
  std::size_t hash_value1 = std::hash<std::string>{}("0123456789");
  std::string hash_password1 = std::to_string(hash_value1);
  tmn_vfs::User* user1 = new tmn_vfs::User(username1, fullname1, hash_password1, tmn_vfs::UserStatus::LOCAL);
  
  std::cout << "CREATION-TIME user1 : " << user1->GetCreationTime() << ";" << std::endl;
  std::cout << "HASH-PASSWORD user1 : " << user1->GetPasswordHash() << ";" << std::endl;
  ASSERT_EQ(user1->GetUserName(), "python-developer OLEG");
  ASSERT_EQ(user1->GetFullName(), "Oleg Petrov");

  delete user1;
}