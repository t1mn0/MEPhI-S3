#include <gtest/gtest.h>

#include <string>

#include "../../include/SmartPtr/UniquePtr.hpp"



// TEST Constructors & assignment for GENERAL REALIZATION :

TEST(TestUniquePtrGeneral, DefaultConstructorByInt) {
  tmn_smart_ptr::UniquePtr<int> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(TestUniquePtrGeneral, DefaultConstructorByString) {
  tmn_smart_ptr::UniquePtr<std::string> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(TestUniquePtrGeneral, PointerConstructorByInt) {
  int* raw_ptr = new int(42);
  tmn_smart_ptr::UniquePtr<int> ptr(raw_ptr);
  ASSERT_EQ(ptr.get(), raw_ptr);
}

TEST(TestUniquePtrGeneral, PointerConstructorByString) {
  std::string* raw_ptr = new std::string("abcdefg");
  tmn_smart_ptr::UniquePtr<std::string> ptr(raw_ptr);
  ASSERT_EQ(ptr.get(), raw_ptr);
}

TEST(TestUniquePtrGeneral, MoveConstructorWithInt) {
  tmn_smart_ptr::UniquePtr<int> ptr_1(new int(10));
  tmn_smart_ptr::UniquePtr<int> ptr_2(std::move(ptr_1));
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), 10);
}

TEST(TestUniquePtrGeneral, MoveConstructorWithString) {
  tmn_smart_ptr::UniquePtr<std::string> ptr_1(new std::string("helloworld"));
  tmn_smart_ptr::UniquePtr<std::string> ptr_2(std::move(ptr_1));
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), "helloworld");
}

TEST(TestUniquePtrGeneral, MoveAssignmentWithInt) {
  tmn_smart_ptr::UniquePtr<int> ptr_1(new int(10));
  tmn_smart_ptr::UniquePtr<int> ptr_2;
  ptr_2 = std::move(ptr_1);
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), 10);
}

TEST(TestUniquePtrGeneral, MoveAssignmentWithString) {
  tmn_smart_ptr::UniquePtr<std::string> ptr_1(new std::string("cplusplus"));
  tmn_smart_ptr::UniquePtr<std::string> ptr_2;
  ptr_2 = std::move(ptr_1);
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_EQ((*ptr_2).value(), "cplusplus");
}



// TEST Modifiers for GENERAL REALIZATION :

TEST(TestUniquePtrGeneral, Release) {
  int* raw_ptr = new int(42);
  tmn_smart_ptr::UniquePtr<int> ptr(raw_ptr);
  int* released_ptr = ptr.release();
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(released_ptr, raw_ptr);
  delete released_ptr;
}

TEST(TestUniquePtrGeneral, Reset) {
  tmn_smart_ptr::UniquePtr<int> ptr(new int(42));
  ptr.reset(new int(10));
  ASSERT_EQ((*ptr).value(), 10);
}

TEST(TestUniquePtrGeneral, Swap) {
  tmn_smart_ptr::UniquePtr<int> ptr_1(new int(1));
  tmn_smart_ptr::UniquePtr<int> ptr_2(new int(2));
  ptr_1.swap(ptr_2);
  ASSERT_EQ((*ptr_1).value(), 2);
  ASSERT_EQ((*ptr_2).value(), 1);
}



// TEST Observers for GENERAL REALIZATION :

TEST(TestUniquePtrGeneral, Get) {
  tmn_smart_ptr::UniquePtr<int> ptr(new int(42));
  ASSERT_EQ(*ptr.get(), 42);
}

TEST(TestUniquePtrGeneral, BoolConversion) {
  tmn_smart_ptr::UniquePtr<int> ptr_1(new int(42));
  tmn_smart_ptr::UniquePtr<int> ptr_2;
  ASSERT_TRUE(static_cast<bool>(ptr_1));
  ASSERT_FALSE(static_cast<bool>(ptr_2));
}



// TEST Constructors & assignment for SPECIALIZATION TEMPLATE :

TEST(TestUniquePtrSpecial, DefaultConstructor) {
  tmn_smart_ptr::UniquePtr<int[]> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
}

TEST(TestUniquePtrSpecial, TemplateConstructor1) {
  int* raw_ptr = new int[5];
  tmn_smart_ptr::UniquePtr<int[]> ptr(raw_ptr);
  ASSERT_EQ(ptr.get(), raw_ptr);
}

TEST(TestUniquePtrSpecial, TemplateConstructor2) {
  int* raw_ptr = new int[10];
  for (int i = 0; i < 10; ++i) { raw_ptr[i] = i + 1; }
  tmn_smart_ptr::UniquePtr<int[]> ptr(raw_ptr);
  ASSERT_EQ(*(ptr.get()), 1);
}

TEST(TestUniquePtrSpecial, MoveConstructor) {
  tmn_smart_ptr::UniquePtr<int[]> ptr_1(new int[10]);
  tmn_smart_ptr::UniquePtr<int[]> ptr_2(std::move(ptr_1));
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_NE(ptr_2.get(), nullptr);
}

TEST(TestUniquePtrSpecial, MoveAssignment) {
  tmn_smart_ptr::UniquePtr<int[]> ptr_1(new int[25]);
  tmn_smart_ptr::UniquePtr<int[]> ptr_2;
  ptr_2 = std::move(ptr_1);
  ASSERT_EQ(ptr_1.get(), nullptr);
  ASSERT_NE(ptr_2.get(), nullptr);
}

// TEST Modifiers for SPECIALIZATION TEMPLATE :

TEST(TestUniquePtrSpecial, Release) {
  tmn_smart_ptr::UniquePtr<int[]> ptr(new int[5]);
  int* raw_ptr = ptr.release();
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_NE(raw_ptr, nullptr);
  delete raw_ptr;
}

TEST(TestUniquePtrSpecial, Reset) {
  tmn_smart_ptr::UniquePtr<int[]> ptr(new int[5]);
  ptr.reset(new int[10]);
  ASSERT_NE(ptr.get(), nullptr);
}

TEST(TestUniquePtrSpecial, Swap) {
  int* raw_ptr_1 = new int[15];
  int* raw_ptr_2 = new int[20];
  tmn_smart_ptr::UniquePtr<int[]> ptr_1(raw_ptr_1);
  tmn_smart_ptr::UniquePtr<int[]> ptr_2(raw_ptr_2);
  ptr_1.swap(ptr_2);
  ASSERT_EQ(ptr_1.get(), raw_ptr_2);
  ASSERT_EQ(ptr_2.get(), raw_ptr_1);
}

TEST(TestUniquePtrSpecial, SwapNullptr) {
  int* raw_ptr = new int[10];
  tmn_smart_ptr::UniquePtr<int[]> ptr_1(raw_ptr);
  tmn_smart_ptr::UniquePtr<int[]> ptr_2;
  ptr_1.swap(ptr_2);
  ASSERT_EQ(ptr_2.get(), raw_ptr);
  ASSERT_EQ(ptr_1.get(), nullptr);
}


// TEST Observers for SPECIALIZATION TEMPLATE :

TEST(TestUniquePtrSpecial, BoolOperator_True) {
  tmn_smart_ptr::UniquePtr<int[]> ptr(new int[5]);
  ASSERT_TRUE(static_cast<bool>(ptr));
}

TEST(TestUniquePtrSpecial, BoolOperator_False) {
  tmn_smart_ptr::UniquePtr<int[]> ptr;
  ASSERT_FALSE(static_cast<bool>(ptr));
}

TEST(TestUniquePtrSpecial, OperatorBracket) {
  int* raw_ptr = new int[5];
  for (int i = 5; i < 10; ++i) { raw_ptr[i-5] = i; }

  tmn_smart_ptr::UniquePtr<int[]> ptr(raw_ptr);

  ASSERT_EQ(ptr[0].value(), 5);
  ASSERT_EQ(ptr[1].value(), 6);
  ASSERT_EQ(ptr[2].value(), 7);
  ASSERT_EQ(ptr[3].value(), 8);
  ASSERT_EQ(ptr[4].value(), 9);
}


// TEST make_unique :

TEST(TestMakeUnique, MakeUniqueWithDefaultConstructor) {
  auto ptr = tmn_smart_ptr::make_unique<int>();
  ASSERT_NE(ptr.get(), nullptr);
}

TEST(TestMakeUnique, MakeUniqueWithArguments) {
  auto ptr = tmn_smart_ptr::make_unique<std::string>("Hello");
  ASSERT_NE(ptr.get(), nullptr);
  ASSERT_EQ(*(ptr.get()), "Hello");
}

TEST(TestMakeUnique, MakeUniqueWithSize) {
  auto ptr = tmn_smart_ptr::make_unique<int>(10);
  ASSERT_NE(ptr.get(), nullptr);
}

TEST(TestMakeUnique, MakeUniqueWithSizeZero) {
  try{
    auto ptr = tmn_smart_ptr::make_unique<int>(0);
  }
  catch(...){
    ASSERT_TRUE(true);
  }
}
