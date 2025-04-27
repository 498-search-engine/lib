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
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, PopBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    Vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);

    v.push_back(5);
    EXPECT_EQ(v[0], 5);
}

TEST(VectorTest, BeginEndIteration) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected);
        expected++;
    }
}

TEST(VectorTest, RangeBasedIteration) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int expected = 1;
    for (auto i : v) {
        EXPECT_EQ(i, expected);
        expected++;
    }
}

TEST(VectorTest, InitializerListConstructor) {
    Vector<int> v = {1, 2, 3, 4, 5};

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5); // capacity could be larger due to growth strategy
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertMiddle) {
    Vector<int> v = {1, 2, 4, 5};
    v.insert(2, 3); // Insert 3 at position 2 (between 2 and 4)

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertFront) {
    Vector<int> v = {2, 3, 4, 5};
    v.insert(0, 1); // Insert 1 at front

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertBack) {
    Vector<int> v = {1, 2, 3, 4};
    v.insert(v.size(), 5); // Insert at end

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertOutOfRangeThrows) {
    Vector<int> v = {1, 2, 3};

    EXPECT_THROW(v.insert(5, 99), std::out_of_range);
}