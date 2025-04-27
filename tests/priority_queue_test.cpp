#include "core/priority_queue.h"
#include <gtest/gtest.h>
#include <vector>

using namespace core;

TEST(PriorityQueueTest, DefaultConstructor) {
    PriorityQueue<int> pq;
    EXPECT_TRUE(pq.empty());
    EXPECT_EQ(pq.size(), 0);
}

TEST(PriorityQueueTest, PushAndTop) {
    PriorityQueue<int> pq;
    pq.push(10);
    EXPECT_EQ(pq.top(), 10);
    pq.push(20);
    EXPECT_EQ(pq.top(), 20);
    pq.push(5);
    EXPECT_EQ(pq.top(), 20);
}

TEST(PriorityQueueTest, PopOperation) {
    PriorityQueue<int> pq;
    pq.push(10);
    pq.push(20);
    pq.push(5);

    EXPECT_EQ(pq.top(), 20);
    pq.pop();
    EXPECT_EQ(pq.top(), 10);
    pq.pop();
    EXPECT_EQ(pq.top(), 5);
    pq.pop();
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, InitializerListConstructor) {
    PriorityQueue<int> pq = {5, 1, 10, 3};
    // pq.printPQ();
    EXPECT_EQ(pq.top(), 10);
    pq.pop();
    // pq.printPQ();
    EXPECT_EQ(pq.top(), 5);
    pq.pop();
    // pq.printPQ();
    EXPECT_EQ(pq.top(), 3);
    pq.pop();
    // pq.printPQ();
    EXPECT_EQ(pq.top(), 1);
}

TEST(PriorityQueueTest, PushRvalue) {
    PriorityQueue<std::string> pq;
    std::string s = "hello";
    pq.push(std::move(s));
    EXPECT_EQ(pq.top(), "hello");
}

TEST(PriorityQueueTest, MultiplePushPop) {
    PriorityQueue<int> pq;
    for (int i = 0; i < 100; ++i) {
        pq.push(i);
    }
    for (int i = 99; i >= 0; --i) {
        EXPECT_EQ(pq.top(), i);
        pq.pop();
    }
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, PopOnEmptyQueue) {
    PriorityQueue<int> pq;
    EXPECT_TRUE(pq.empty());
    EXPECT_NO_THROW(pq.pop());
    EXPECT_TRUE(pq.empty());
}
