#include <gtest/gtest.h>

#include "../../include/VFS/Group.hpp"


TEST(GroupTest, TestGroupCreationAndGetters) {
  tmn_vfs::Group* group1 = new tmn_vfs::Group("group1", "creater1", "hash1");
  tmn_vfs::Group* group2 = new tmn_vfs::Group("group2", "creater2", "hash2");
  tmn_vfs::Group* group3 = new tmn_vfs::Group("group3", "creater3","hash3");

  ASSERT_EQ(group1->GetGroupName(), "group1");
  ASSERT_EQ(group2->GetGroupName(), "group2");
  ASSERT_EQ(group3->GetGroupName(), "group3");

  ASSERT_EQ(group1->GetPasswordHash(), "hash1");
  ASSERT_EQ(group2->GetPasswordHash(), "hash2");
  ASSERT_EQ(group3->GetPasswordHash(), "hash3");

  delete group1;
  delete group2;
  delete group3;
}