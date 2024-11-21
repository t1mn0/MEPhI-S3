#include <gtest/gtest.h>

#include "../../include/VFS/Group.hpp"


TEST(GroupTest, Constructors) {
  tmn_vfs::Group* group1 = new tmn_vfs::Group("group1", "creater1");
  tmn_vfs::Group* group2 = new tmn_vfs::Group("group2", "creater2");
  tmn_vfs::Group* group3 = new tmn_vfs::Group("group3", "creater3");

  ASSERT_EQ(group1->GroupName(), "group1");
  ASSERT_EQ(group2->GroupName(), "group2");
  ASSERT_EQ(group3->GroupName(), "group3");

  ASSERT_EQ(group1->OwnerName(), "creater1");
  ASSERT_EQ(group2->OwnerName(), "creater2");
  ASSERT_EQ(group3->OwnerName(), "creater3");

  delete group1;
  delete group2;
  delete group3;
}