#include "core/array.h"

#include <gtest/gtest.h>

using namespace core;

TEST(Array, Initialize) {
    Array<int, 3> a = {1, 2, 3};
    ASSERT_EQ(a.Size(), 3);
    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a[2], 3);
}

TEST(Array, Copy) {
    Array<int, 3> a = {1, 2, 3};
    auto b = a;
    ASSERT_EQ(b.Size(), 3);
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
    ASSERT_EQ(b[2], 3);

    a[1] = 10;

    ASSERT_EQ(a[1], 10);
    ASSERT_EQ(b[1], 2);
}

TEST(Array, Fill) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    a.Fill(10);
    ASSERT_EQ(a[0], 10);
    ASSERT_EQ(a[1], 10);
    ASSERT_EQ(a[2], 10);
    ASSERT_EQ(a[3], 10);
    ASSERT_EQ(a[4], 10);
}

TEST(Array, Iterator) {
    Array<int, 3> a = {1, 2, 3};
    auto* it = a.begin();
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 1);
    ++it;
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 3);
    ++it;
    ASSERT_EQ(it, a.end());
}

TEST(Array, Swap) {
    Array<int, 3> a = {1, 2, 3};
    Array<int, 3> b = {4, 5, 6};

    a.Swap(b);

    ASSERT_EQ(a[0], 4);
    ASSERT_EQ(a[1], 5);
    ASSERT_EQ(a[2], 6);
    ASSERT_EQ(b[0], 1);
    ASSERT_EQ(b[1], 2);
    ASSERT_EQ(b[2], 3);
}
