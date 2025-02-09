#include <gtest/gtest.h>

#include <string>

#include "../../include/SmartPtr/SharedPtr.hpp"



// TEST Constructors & assignment :

TEST(SharedPtrTest, DefaultConstructor) {
  tmn::smart_ptr::SharedPtr<int> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(SharedPtrTest, ConstructorAndCopy) {
  int* raw_ptr = new int(10);
  tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr);
  ASSERT_EQ(ptr_1.get(), raw_ptr);
  ASSERT_EQ(ptr_1.use_count(), 1);
  tmn::smart_ptr::SharedPtr<int> ptr_2(ptr_1);
  tmn::smart_ptr::SharedPtr<int> ptr_3(ptr_2);
  tmn::smart_ptr::SharedPtr<int> ptr_4(ptr_3);
  tmn::smart_ptr::SharedPtr<int> ptr_5(ptr_4);
  ASSERT_EQ(ptr_1.get(), ptr_2.get());
  ASSERT_EQ(ptr_2.get(), ptr_3.get());
  ASSERT_EQ(ptr_3.get(), ptr_4.get());
  ASSERT_EQ(ptr_4.get(), ptr_5.get());
  ASSERT_EQ(ptr_1.use_count(), 5);
  ASSERT_EQ(ptr_3.use_count(), 5);
  ASSERT_EQ(ptr_5.use_count(), 5);
}

TEST(SharedPtrTest, CopyAssignment) {
  int* raw_ptr1 = new int(10);
  int* raw_ptr2 = new int(22);
  tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr1);
  tmn::smart_ptr::SharedPtr<int> ptr_2(raw_ptr2);
  std::cout << std::endl;
  std::cout << "USE COUNT1 : " << ptr_1.use_count() << std::endl;
  std::cout << std::endl;
  std::cout << "USE COUNT2 : " << ptr_2.use_count() << std::endl;
  ptr_2 = ptr_1;
  std::cout << std::endl;
  std::cout << "USE COUNT1 : " << ptr_1.use_count() << std::endl;
  std::cout << std::endl;
  std::cout << "USE COUNT2 : " << ptr_2.use_count() << std::endl;
  ASSERT_EQ(ptr_1.get(), ptr_2.get());
  ASSERT_EQ((*ptr_1).value(), (*ptr_2).value());
}

TEST(SharedPtrTest, MoveConstructor) {
  int* raw_ptr = new int(10);
  tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr);
  tmn::smart_ptr::SharedPtr<int> ptr_2(std::move(ptr_1));
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ(ptr_2.get(), raw_ptr);
}

TEST(SharedPtrTest, MoveAssignment) {
    int* raw_ptr1 = new int(10);
    int* raw_ptr2 = new int(22);
    tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr1);
    tmn::smart_ptr::SharedPtr<int> ptr_2(raw_ptr2);
  ptr_2 = std::move(ptr_1);
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ(ptr_2.get(), raw_ptr1);
}



// TEST Modifiers :

TEST(SharedPtrTest, Reset) {
  int* raw_ptr1 = new int(100);
  int* raw_ptr2 = new int(200);
  tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr1);
  tmn::smart_ptr::SharedPtr<int> ptr_2(ptr_1);
  tmn::smart_ptr::SharedPtr<int> ptr_3(ptr_1);
  ASSERT_EQ(ptr_2.use_count(), 3);

  ptr_1.reset(raw_ptr2);
  ASSERT_EQ(ptr_1.use_count(), 1);
  ASSERT_EQ(ptr_2.use_count(), 2);
  ASSERT_EQ(ptr_3.use_count(), 2);
  ASSERT_EQ(ptr_1.get(), raw_ptr2);
  ASSERT_EQ(ptr_3.get(), raw_ptr1);
}

TEST(SharedPtrTest, Swap) {
  int* raw_ptr1 = new int(144);
  int* raw_ptr2 = new int(169);
  tmn::smart_ptr::SharedPtr<int> ptr_1(raw_ptr1);
  tmn::smart_ptr::SharedPtr<int> ptr_2(raw_ptr2);
  tmn::smart_ptr::SharedPtr<int> ptr_3(ptr_2);
  ptr_1.swap(ptr_2);
  ASSERT_EQ(ptr_1.get(), raw_ptr2);
  ASSERT_EQ(ptr_2.get(), raw_ptr1);
  ASSERT_EQ(ptr_1.use_count(), 2);
  ASSERT_EQ(ptr_2.use_count(), 1);
  ASSERT_EQ(ptr_3.use_count(), 2);
}



// TEST Observers :

TEST(SharedPtrTest, BoolOperatorTrue) {
  int* raw_ptr = new int(10);
  tmn::smart_ptr::SharedPtr<int> ptr(raw_ptr);
  ASSERT_TRUE(static_cast<bool>(ptr));
}

TEST(SharedPtrTest, BoolOperatorFalse) {
  tmn::smart_ptr::SharedPtr<int> ptr;
  ASSERT_FALSE(static_cast<bool>(ptr));
}

TEST(SharedPtrTest, DereferenceOperator) {
  int* raw_ptr = new int(10);
  tmn::smart_ptr::SharedPtr<int> ptr(raw_ptr);
  ASSERT_EQ((*ptr).value(), 10);
  ASSERT_EQ((*ptr).value(), *(ptr.get()));
}

TEST(SharedPtrTest, UseCount) {
  int* raw_ptr = new int(10);
  tmn::smart_ptr::SharedPtr<int> ptr1(raw_ptr);
  tmn::smart_ptr::SharedPtr<int> ptr2(ptr1);
  tmn::smart_ptr::SharedPtr<int> ptr3(ptr2);
  tmn::smart_ptr::SharedPtr<int> ptr4(ptr2);
  ASSERT_EQ(ptr1.use_count(), 4);
  ASSERT_EQ(ptr2.use_count(), 4);
  ASSERT_EQ(ptr3.use_count(), 4);
  ASSERT_EQ(ptr4.use_count(), 4);
}

TEST(SharedPtrTest, Unique) {
  int* raw_ptr = new int(512);
  tmn::smart_ptr::SharedPtr<int> ptr1(raw_ptr);
  ASSERT_TRUE(ptr1.unique());
  tmn::smart_ptr::SharedPtr<int> ptr2(ptr1);
  ASSERT_FALSE(ptr1.unique());
  ASSERT_FALSE(ptr2.unique());
}
