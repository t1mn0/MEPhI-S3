#include <gtest/gtest.h>

#include "../../include/VFS/Group.hpp"

TEST(GroupTest, BaseGroup) {
  std::string groupname1 = "Linux users";
  std::string password_hash = "TempleOS";
  tmn_vfs::Group* group1 = new tmn_vfs::Group(groupname1, password_hash);
  
  std::cout << "CREATION-TIME group1 : " << group1->GetCreationTime() << ";" << std::endl;
  ASSERT_EQ(group1->GetGroupName(), "Linux users");

  delete group1;
}