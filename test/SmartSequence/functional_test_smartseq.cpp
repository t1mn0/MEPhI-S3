#include <gtest/gtest.h>

#include <string>

#include "../../include/Sort/SmartSequence.hpp"
#include "../../include/Pair/Pair.hpp"

TEST(TestSmartSequence, DefaultConstructor) {
  tmn::sequence::SmartSequence<int> seq;
  ASSERT_TRUE(seq.empty());
  ASSERT_EQ(seq.size(), 0);
  ASSERT_EQ(seq.capacity(), 4);
}


TEST(TestSmartSequence, SizeConstructor) {
  tmn::sequence::SmartSequence<int> seq(5);
  ASSERT_FALSE(seq.empty());
  ASSERT_EQ(seq.size(), 5);
  ASSERT_EQ(seq.capacity(),  10);
}

TEST(TestSmartSequence, SizeConstructorWithDefaultElement) {
  tmn::sequence::SmartSequence<std::string> seq(3, "default");
  ASSERT_FALSE(seq.empty());
  ASSERT_EQ(seq.size(), 3);
  for (size_t i = 0; i < seq.size(); ++i) {
    ASSERT_EQ(seq[i], "default");
  }
}

TEST(TestSmartSequence, CopyConstructor) {
  tmn::sequence::SmartSequence<int> seq1(3, 10);
  tmn::sequence::SmartSequence<int> seq2(seq1);

  ASSERT_EQ(seq1.size(), seq2.size());
  ASSERT_EQ(seq1.capacity(), seq2.capacity());
  for (size_t i = 0; i < seq1.size(); ++i) {
    ASSERT_EQ(seq1[i], seq2[i]);
  }

  seq2.push_back(11);
  ASSERT_NE(seq1.size(), seq2.size());
}

TEST(TestSmartSequence, MoveConstructor) {
  tmn::sequence::SmartSequence<std::string> seq1(3, "test");
  tmn::sequence::SmartSequence<std::string> seq2(std::move(seq1));
  
  ASSERT_EQ(seq2.size(), 3);
  ASSERT_EQ(seq2.capacity(), 6);

  for (size_t i = 0; i < seq2.size(); ++i){
    ASSERT_EQ(seq2[i], "test");
  }

  ASSERT_EQ(seq1.size(), 0);
  ASSERT_EQ(seq1.capacity(), 0);
}


TEST(TestSmartSequence, InitializerListConstructor) {
  tmn::sequence::SmartSequence<double> seq = {1.1, 2.2, 3.3};
  ASSERT_EQ(seq.size(), 3);
  ASSERT_EQ(seq.capacity(), 6);
  ASSERT_EQ(seq[0], 1.1);
  ASSERT_EQ(seq[1], 2.2);
  ASSERT_EQ(seq[2], 3.3);
}

TEST(TestSmartSequence, SwapMethod) {
  tmn::sequence::SmartSequence<int> seq1{1, 2, 3};
  tmn::sequence::SmartSequence<int> seq2{4, 5};

  seq1.swap(seq2);
  ASSERT_EQ(seq1.size(), 2);
  ASSERT_EQ(seq1[0], 4);
  ASSERT_EQ(seq1[1], 5);

  ASSERT_EQ(seq2.size(), 3);
  ASSERT_EQ(seq2[0], 1);
  ASSERT_EQ(seq2[1], 2);
  ASSERT_EQ(seq2[2], 3);
}

TEST(TestSmartSequence, AssignmentOperator) {
  tmn::sequence::SmartSequence<int> seq1 = {1, 2, 3};
  tmn::sequence::SmartSequence<int> seq2;
  
  seq2 = seq1;
  ASSERT_EQ(seq1.size(), seq2.size());
  
  for (size_t i = 0; i < seq1.size(); ++i) {
    ASSERT_EQ(seq1[i], seq2[i]);
  }

  seq2.push_back(4);
  ASSERT_NE(seq1.size(), seq2.size());
}

// --- Capacity & size ---

TEST(TestSmartSequence, ReserveMethod) {
  tmn::sequence::SmartSequence<int> seq;
  seq.reserve(10);
  ASSERT_GE(seq.capacity(), 10);
}

TEST(TestSmartSequence, ResizeMethod) {
  tmn::sequence::SmartSequence<int> seq;
  seq.resize(5);
  ASSERT_EQ(seq.size(), 5);
  ASSERT_EQ(seq.capacity(), 10);

  tmn::sequence::SmartSequence<std::string> seq2;
  seq2.resize(3, "test");
  ASSERT_EQ(seq2.size(), 3);
  ASSERT_EQ(seq2.capacity(), 4);

  for(size_t i = 0; i < seq2.size(); i++) {
    ASSERT_EQ(seq2[i], "test");
  }
}

TEST(TestSmartSequence, ClearMethod) {
  tmn::sequence::SmartSequence<int> seq = {1, 2, 3, 4};
  seq.clear();
  ASSERT_TRUE(seq.empty());
  ASSERT_EQ(seq.size(), 0);
}


TEST(TestSmartSequence, PushBackMethod) {
  tmn::sequence::SmartSequence<int> seq;
  seq.push_back(10);
  ASSERT_EQ(seq.size(), 1);
  ASSERT_EQ(seq[0], 10);
  seq.push_back(11);
  ASSERT_EQ(seq[1], 11);

  tmn::sequence::SmartSequence<std::string> seq2;
  seq2.push_back("test1");
  ASSERT_EQ(seq2.size(), 1);
  ASSERT_EQ(seq2[0], "test1");
  seq2.push_back("test2");
  ASSERT_EQ(seq2[1], "test2");
}

TEST(TestSmartSequence, PopBackMethod) {
  tmn::sequence::SmartSequence<int> seq = {1, 2 ,3};
  seq.pop_back();
  ASSERT_EQ(seq.size(), 2);
  ASSERT_EQ(seq[0], 1);
  ASSERT_EQ(seq[1], 2);

  tmn::sequence::SmartSequence<int> seq2;
  ASSERT_THROW(seq2.pop_back(), tmn::exception::LogicException);
  
  tmn::sequence::SmartSequence<int> seq3{1};
  seq3.pop_back();
  ASSERT_TRUE(seq3.empty());
}


TEST(TestSmartSequence, EmplaceBackMethod) {
  tmn::sequence::SmartSequence<tmn::Pair<int, std::string>> seq;
  seq.emplace_back(1, "test");
  ASSERT_EQ(seq.size(), 1);
  ASSERT_EQ(seq[0].first, 1);
  ASSERT_EQ(seq[0].second, "test");
}

// --- Element access methods ---

TEST(TestSmartSequence, FrontMethod) {
  tmn::sequence::SmartSequence<int> seq = {1, 2 ,3};
  ASSERT_EQ(seq.front(), 1);
  seq.front() = 4;
  ASSERT_EQ(seq.front(), 4);

  const tmn::sequence::SmartSequence<int> seq2 = {5, 6, 7};
  ASSERT_EQ(seq2.front(), 5);

  tmn::sequence::SmartSequence<int> seq3;
  ASSERT_THROW(seq3.front(), tmn::exception::LogicException);
}


TEST(TestSmartSequence, BackMethod) {
  tmn::sequence::SmartSequence<int> seq = {1, 2 ,3};
  ASSERT_EQ(seq.back(), 3);
  seq.back() = 4;
  ASSERT_EQ(seq.back(), 4);

  const tmn::sequence::SmartSequence<int> seq2{5, 6, 7};
  ASSERT_EQ(seq2.back(), 7);

  tmn::sequence::SmartSequence<int> seq3;
  ASSERT_THROW(seq3.back(), tmn::exception::LogicException);
}

TEST(TestSmartSequence, SetMethod) {
  tmn::sequence::SmartSequence<int> seq = {1, 2 ,3};
  seq.set(1, 10);
  ASSERT_EQ(seq[1], 10);
  ASSERT_EQ(seq.get(1), 10);

  ASSERT_THROW(seq.set(10, 10), tmn::exception::LogicException);

  tmn::sequence::SmartSequence<std::string> seq2 = {"1", "2" ,"3"};
  seq2.set(1, "10");
  ASSERT_EQ(seq2[1], "10");
  ASSERT_EQ(seq2[0], "1");
  ASSERT_EQ(seq2[2], "3");
}