// d-ary heap tests
// authors: @mdvsh


#include <gtest/gtest.h>
#include "core/dary_heap.h"

// Test fixture for dary_heap
class DaryHeapTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize a heap with arity D = 4 (default)
        heap = dary_heap<int, int>{};
    }

    dary_heap<int, int> heap;
};

// Test basic push and top functionality
TEST_F(DaryHeapTest, PushAndTop) {
    heap.push(10, 5);
    EXPECT_EQ(heap.top(), 10);

    heap.push(20, 3);
    EXPECT_EQ(heap.top(), 20); // 20 has higher priority (lower value)

    heap.push(30, 7);
    EXPECT_EQ(heap.top(), 20); // 20 still has the highest priority
}

// Test pop functionality
TEST_F(DaryHeapTest, Pop) {
    heap.push(10, 5);
    heap.push(20, 3);
    heap.push(30, 7);

    heap.pop();
    EXPECT_EQ(heap.top(), 10); // After popping 20, 10 is now the top

    heap.pop();
    EXPECT_EQ(heap.top(), 30); // After popping 10, 30 is now the top
}

// Test updatePriority functionality
TEST_F(DaryHeapTest, UpdatePriority) {
    heap.push(10, 5);
    heap.push(20, 3);
    heap.push(30, 7);

    heap.updatePriority(10, 1); // Update priority of 10 to 1 (highest priority)
    EXPECT_EQ(heap.top(), 10);   // 10 should now be at the top

    heap.updatePriority(30, 2); // Update priority of 30 to 2
    heap.pop();                  // Pop 10
    EXPECT_EQ(heap.top(), 30);   // 30 should now be at the top
}

// Test contains functionality
TEST_F(DaryHeapTest, Contains) {
    heap.push(10, 5);
    heap.push(20, 3);

    EXPECT_TRUE(heap.contains(10));
    EXPECT_TRUE(heap.contains(20));
    EXPECT_FALSE(heap.contains(30)); // 30 is not in the heap
}

// Test empty and size functionality
TEST_F(DaryHeapTest, EmptyAndSize) {
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(heap.size(), 0);

    heap.push(10, 5);
    EXPECT_FALSE(heap.empty());
    EXPECT_EQ(heap.size(), 1);

    heap.push(20, 3);
    EXPECT_EQ(heap.size(), 2);

    heap.pop();
    EXPECT_EQ(heap.size(), 1);

    heap.pop();
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(heap.size(), 0);
}

// Test duplicate push (should throw)
TEST_F(DaryHeapTest, DuplicatePush) {
    heap.push(10, 5);
    EXPECT_THROW(heap.push(10, 3), std::logic_error); // Duplicate value should throw
}

// Test pop on empty heap (should throw)
TEST_F(DaryHeapTest, PopEmptyHeap) {
    EXPECT_THROW(heap.pop(), std::out_of_range); // Pop on empty heap should throw
}

// Test top on empty heap (should throw)
TEST_F(DaryHeapTest, TopEmptyHeap) {
    EXPECT_THROW(static_cast<void>(heap.top()), std::out_of_range); // Top on empty heap should throw
}
// Test updatePriority on non-existent value (should throw)
TEST_F(DaryHeapTest, UpdatePriorityNonExistent) {
    heap.push(10, 5);
    EXPECT_THROW(heap.updatePriority(20, 3), std::invalid_argument); // 20 is not in the heap
}

// Test heap property after multiple operations
TEST_F(DaryHeapTest, HeapProperty) {
    heap.push(10, 5);
    heap.push(20, 3);
    heap.push(30, 7);
    heap.push(40, 2);
    heap.push(50, 6);

    EXPECT_EQ(heap.top(), 40); // 40 has the highest priority (lowest value)

    heap.pop();
    EXPECT_EQ(heap.top(), 20); // 20 is now the top

    heap.updatePriority(30, 1); // Update priority of 30 to 1
    EXPECT_EQ(heap.top(), 30);   // 30 should now be at the top

    heap.pop();
    EXPECT_EQ(heap.top(), 20); // 20 is now the top again
}

// Test custom arity (D = 2, binary heap)
TEST(DaryHeapCustomArityTest, BinaryHeap) {
    dary_heap<int, int, 2> binary_heap;

    binary_heap.push(10, 5);
    binary_heap.push(20, 3);
    binary_heap.push(30, 7);

    EXPECT_EQ(binary_heap.top(), 20); // 20 has the highest priority

    binary_heap.pop();
    EXPECT_EQ(binary_heap.top(), 10); // 10 is now the top
}

// Test custom comparator (max-heap)
TEST(DaryHeapCustomComparatorTest, MaxHeap) {
    dary_heap<int, int, 4, std::greater<int>> max_heap;

    max_heap.push(10, 5);
    max_heap.push(20, 3);
    max_heap.push(30, 7);

    EXPECT_EQ(max_heap.top(), 30); // 30 has the highest priority (max-heap)

    max_heap.pop();
    EXPECT_EQ(max_heap.top(), 10); // 10 is now the top
}