// PairTest.cpp
#include "gtest/gtest.h"
#include "core/pair.h" 
#include <string>

using namespace core;

// Basic creation test
TEST(PairTest, BasicInitialization) {
    Pair<int, std::string> p{42, "mithril"};

    EXPECT_EQ(p.first, 42);
    EXPECT_EQ(p.second, "mithril");
}

// Structured binding test
TEST(PairTest, StructuredBinding) {
    Pair<int, std::string> p{99, "feather"};

    auto& [num, str] = p;

    EXPECT_EQ(num, 99);
    EXPECT_EQ(str, "feather");

    // Modifying through structured binding should affect original
    num = 100;
    str = "dragon";

    EXPECT_EQ(p.first, 100);
    EXPECT_EQ(p.second, "dragon");
}

// Const structured binding
TEST(PairTest, ConstStructuredBinding) {
    const Pair<int, double> p{7, 3.14};

    auto [x, y] = p;

    EXPECT_EQ(x, 7);
    EXPECT_DOUBLE_EQ(y, 3.14);
}

// Different types
TEST(PairTest, DifferentTypes) {
    Pair<char, bool> p{'x', true};

    EXPECT_EQ(p.first, 'x');
    EXPECT_TRUE(p.second);
}

// Nested Pair
TEST(PairTest, NestedPair) {
    Pair<int, Pair<std::string, double>> p{1, {"pi", 3.1415}};

    EXPECT_EQ(p.first, 1);
    EXPECT_EQ(p.second.first, "pi");
    EXPECT_NEAR(p.second.second, 3.1415, 1e-5);
}
