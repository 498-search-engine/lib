#include "gtest/gtest.h"
#include "core/deque.h"

using namespace core;

TEST(DequeTest, PushBackAndFront) {
    Deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);

    EXPECT_EQ(d.front(), 0);
    EXPECT_EQ(d.back(), 2);
    EXPECT_EQ(d.size(), 3);
}

TEST(DequeTest, PopBackAndFront) {
    Deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);

    d.pop_front();
    EXPECT_EQ(d.front(), 1);
    d.pop_back();
    EXPECT_EQ(d.back(), 1);
    d.pop_back();
    EXPECT_TRUE(d.empty());
}

TEST(DequeTest, OperatorAccess) {
    Deque<int> d;
    for (int i = 0; i < 100; ++i) {
        d.push_back(i);
    }

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(d[i], i);
    }
}

TEST(DequeTest, SwapFunctionality) {
    Deque<int> d1;
    d1.push_back(1);
    d1.push_back(2);

    Deque<int> d2;
    d2.push_back(3);
    d2.push_back(4);

    d1.swap(d2);

    EXPECT_EQ(d1.front(), 3);
    EXPECT_EQ(d1.back(), 4);
    EXPECT_EQ(d2.front(), 1);
    EXPECT_EQ(d2.back(), 2);
}

TEST(DequeTest, IteratorTraversal) {
    Deque<int> d;
    for (int i = 0; i < 10; ++i) {
        d.push_back(i);
    }

    int expected = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

