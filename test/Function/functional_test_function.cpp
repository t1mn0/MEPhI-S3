#include "gtest/gtest.h"

#include "../../include/Function/Function.hpp"

TEST(FunctionTest, DefaultConstructor) {
  tmn::Function<void()> func;
  ASSERT_FALSE(static_cast<bool>(func));
}

TEST(FunctionTest, NullptrConstructor) {
  tmn::Function<void()> func(nullptr);
  ASSERT_FALSE(static_cast<bool>(func));
}

TEST(FunctionTest, FunctionPointerConstructor) {
  int x = 5;
  auto increment = [](int& value) { value++; };
  tmn::Function<void(int&)> func(increment);
  ASSERT_TRUE(static_cast<bool>(func));

  func(x);
  ASSERT_EQ(x, 6);
}

TEST(FunctionTest, LambdaConstructor) {
  auto add = [](int a, int b) { return a + b; };
  tmn::Function<int(int, int)> func(add);
  ASSERT_TRUE(static_cast<bool>(func));
  ASSERT_EQ(func(2, 3), 5);
}

TEST(FunctionTest, CopyConstructor) {
  auto add = [](int a, int b) { return a + b; };
  tmn::Function<int(int, int)> func1(add);
  tmn::Function<int(int, int)> func2(func1);
  ASSERT_TRUE(static_cast<bool>(func1));
  ASSERT_TRUE(static_cast<bool>(func2));
  ASSERT_EQ(func2(2, 3), 5); 
}

TEST(FunctionTest, MoveConstructor) {
  auto add = [](int a, int b) { return a + b; };
  tmn::Function<int(int, int)> func1(add);
  tmn::Function<int(int, int)> func2(std::move(func1));
  ASSERT_FALSE(static_cast<bool>(func1));
  ASSERT_TRUE(static_cast<bool>(func2));
  ASSERT_EQ(func2(2, 3), 5);
}

// TEST(FunctionTest, CopyAssignment) {
//   auto add1 = [](int a, int b) { return a + b; };
//   auto add2 = [](int a, int b) { return a - b; };
//   tmn::Function<int(int, int)> func1(add1);
//   tmn::Function<int(int, int)> func2(add2);

//   func1 = func2;
//   ASSERT_TRUE(static_cast<bool>(func1));
//   ASSERT_EQ(func1(5, 3), 2); 
// }

TEST(FunctionTest, MoveAssignment) {
  auto add1 = [](int a, int b) { return a + b; };
  auto add2 = [](int a, int b) { return a - b; };
  tmn::Function<int(int, int)> func1(add1);
  tmn::Function<int(int, int)> func2(add2);

  func1 = std::move(func2);
  ASSERT_FALSE(static_cast<bool>(func2));
  ASSERT_TRUE(static_cast<bool>(func1));
  ASSERT_EQ(func1(5, 3), 2); 
}

TEST(FunctionTest, FunctionCall) {
  auto add = [](int a, int b) { return a + b; };
  tmn::Function<int(int, int)> func(add);
  ASSERT_EQ(func(2, 3), 5);
}

TEST(FunctionTest, InvalidCall) {
  tmn::Function<void()> func;
  ASSERT_THROW(func(), tmn::exception::LogicException); 
}