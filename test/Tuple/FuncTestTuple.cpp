#include "gtest/gtest.h"

#include<iostream>

#include "../../include/Tuple/Tuple.hpp"

TEST(TupleTest, SingleElementTuple) {
    tmn_tuple::Tuple<int> int_tuple(10);
    ASSERT_EQ(int_tuple.size(), 1);

    tmn_tuple::Tuple<std::string> string_tuple("hello");
    ASSERT_EQ(string_tuple.size(), 1);

    tmn_tuple::Tuple<double> double_tuple(3.14159);
    ASSERT_EQ(double_tuple.size(), 1);
}

TEST(TupleTest, CopyConstructor) {
    tmn_tuple::Tuple<int, std::string> original(1, "test");
    tmn_tuple::Tuple<int, std::string> copied = original;
    ASSERT_EQ(original.size(), 2);
    ASSERT_EQ(copied.size(), 2);
    
    ASSERT_EQ(tmn_tuple::get<1>(original), 1);
    ASSERT_EQ(tmn_tuple::get<1>(copied), 1);
    ASSERT_EQ(tmn_tuple::get<std::string>(original), tmn_tuple::get<std::string>(copied));
}

TEST(TupleTest, MoveConstructor) {
    tmn_tuple::Tuple<int, std::string> original(1, "test");
    tmn_tuple::Tuple<int, std::string> moved = std::move(original);
    ASSERT_EQ(original.size(), 2);
    ASSERT_EQ(moved.size(), 2);
    ASSERT_EQ(tmn_tuple::get<1>(moved), 1);
    ASSERT_EQ(tmn_tuple::get<2>(moved), "test");
}

TEST(TupleTest, SizeTuple) {
    tmn_tuple::Tuple<int, double> tuple1(10, 144.169);
    ASSERT_EQ(tuple1.size(), 2);

    tmn_tuple::Tuple<int, int, std::string> tuple2(10, 144, "it is tuple");
    ASSERT_EQ(tuple2.size(), 3);

    tmn_tuple::Tuple<double, double, double, const char*, const char*, const char*> tuple3(1.5, 6.5, 12.22, "OX", "OY", "OZ");
    ASSERT_EQ(tuple3.size(), 6);
}

TEST(TupleTest, GetByType) {
    tmn_tuple::Tuple<int, double, const char*> tuple(10, 144.169, "it is char*? no, it is const char*");
    
    ASSERT_EQ(tmn_tuple::get<double>(tuple), 144.169);
    ASSERT_EQ(tmn_tuple::get<int>(tuple), 10);
    ASSERT_EQ(tmn_tuple::get<const char*>(tuple), "it is char*? no, it is const char*");
}

TEST(TupleTest, GetByNum) {
    tmn_tuple::Tuple<int, double, const char*> tuple(10, 144.169, "it is char*? no, it is const char*");
    
    ASSERT_EQ(tmn_tuple::get<2>(tuple), 144.169);
    ASSERT_EQ(tmn_tuple::get<1>(tuple), 10);
    ASSERT_EQ(tmn_tuple::get<3>(tuple), "it is char*? no, it is const char*");
}