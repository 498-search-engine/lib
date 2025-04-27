#include "core/vector.h" 
#include <gtest/gtest.h>
#include <string>

using core::Vector;

TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, ResizeConstructor) {
    Vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorTest, FillConstructor) {
    Vector<std::string> v(3, "hello");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], "hello");
    }
}

TEST(VectorTest, CopyConstructor) {
    Vector<int> v1(4, 7);
    Vector<int> v2(v1);

    EXPECT_EQ(v2.size(), 4);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 7);
    }
}

TEST(VectorTest, CopyAssignment) {
    Vector<int> v1(2, 42);
    Vector<int> v2;
    v2 = v1;

    EXPECT_EQ(v2.size(), 2);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 42);
    }
}

TEST(VectorTest, MoveConstructor) {
    Vector<int> v1(3, 9);
    Vector<int> v2(std::move(v1));

    EXPECT_EQ(v2.size(), 3);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 9);
    }
    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, MoveAssignment) {
    Vector<int> v1(2, 5);
    Vector<int> v2;
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 2);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 5);
    }
    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, PushBack) {
    Vector<int> v;
    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);

    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, PopBack) {
    Vector<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.popBack();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    Vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);

    v.pushBack(5);
    EXPECT_EQ(v[0], 5);
}

TEST(VectorTest, BeginEndIteration) {
    Vector<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);

    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected);
        expected++;
    }
}

TEST(VectorTest, RangeBasedIteration) {
    Vector<int> v;
    v.pushBack(1);
    v.pushBack(2);
    v.pushBack(3);

    int expected = 1;
    for (auto i : v) {
        EXPECT_EQ(i, expected);
        expected++;
    }
}
