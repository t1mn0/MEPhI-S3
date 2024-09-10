#include <gtest/gtest.h>

#include "../../include/SmartPtr/UniquePtr.hpp"


// TEST Constructors & assignment :

TEST(UniquePtrTest, DefaultConstructor) {
  tmn_smrt_ptr::UniquePtr<int> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(UniquePtrTest, PointerConstructor) {
  int* raw_ptr = new int(42);
  tmn_smrt_ptr::UniquePtr<int> ptr(raw_ptr);
  ASSERT_EQ(ptr.get(), raw_ptr);
}

TEST(UniquePtrTest, MoveConstructor) {
  tmn_smrt_ptr::UniquePtr<int> ptr_1(new int(10));
  tmn_smrt_ptr::UniquePtr<int> ptr_2(std::move(ptr_1));
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), 10); 
}

TEST(UniquePtrTest, MoveAssignment) {
  tmn_smrt_ptr::UniquePtr<int> ptr_1(new int(10));
  tmn_smrt_ptr::UniquePtr<int> ptr_2;
  ptr_2 = std::move(ptr_1);
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), 10);
}



// TEST Modifiers :

TEST(UniquePtrTest, Release) {
  int* raw_ptr = new int(42);
  tmn_smrt_ptr::UniquePtr<int> ptr(raw_ptr);
  int* released_ptr = ptr.release();
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(released_ptr, raw_ptr);
  delete released_ptr;
}

TEST(UniquePtrTest, Reset) {
  tmn_smrt_ptr::UniquePtr<int> ptr(new int(42));
  ptr.reset(new int(10));
  ASSERT_EQ((*ptr).value(), 10); 
}

TEST(UniquePtrTest, Swap) {
  tmn_smrt_ptr::UniquePtr<int> ptr_1(new int(1));
  tmn_smrt_ptr::UniquePtr<int> ptr_2(new int(2));
  ptr_1.swap(ptr_2);
  ASSERT_EQ((*ptr_1).value(), 2);
  ASSERT_EQ((*ptr_2).value(), 1);
}



// Observers :

TEST(UniquePtrTest, Get) {
  tmn_smrt_ptr::UniquePtr<int> ptr(new int(42));
  ASSERT_EQ(*ptr.get(), 42);
}

TEST(UniquePtrTest, BoolConversion) {
  tmn_smrt_ptr::UniquePtr<int> ptr_1(new int(42));
  tmn_smrt_ptr::UniquePtr<int> ptr_2;
  ASSERT_TRUE(static_cast<bool>(ptr_1));
  ASSERT_FALSE(static_cast<bool>(ptr_2));
}

