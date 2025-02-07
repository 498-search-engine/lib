#include <gtest/gtest.h>
#include <core/bloom_filter.h>
#include <stdexcept>

using namespace core;

class BloomfilterTest : public ::testing::Test {
protected:
    // Common test parameters
    const int default_capacity = 1000;
    const double default_error_rate = 0.01;
};

// Test basic insertion and querying
TEST_F(BloomfilterTest, BasicOperation) {
    Bloomfilter filter(default_capacity, default_error_rate);
    
    filter.insert("hello");
    EXPECT_TRUE(filter.contains("hello"));
    EXPECT_FALSE(filter.contains("world")); // Not inserted, should be absent
}

// Test constructor parameter validation
TEST_F(BloomfilterTest, ConstructorValidation) {
    EXPECT_THROW(Bloomfilter(-1, default_error_rate), std::runtime_error);
    EXPECT_THROW(Bloomfilter(default_capacity, 0.0), std::runtime_error);
    EXPECT_THROW(Bloomfilter(default_capacity, 1.0), std::runtime_error);
}

// Test memory usage is reasonable
TEST_F(BloomfilterTest, MemoryUsage) {
    Bloomfilter filter(default_capacity, default_error_rate);
    // Memory should be non-zero and a multiple of sizeof(uint64_t)
    EXPECT_GT(filter.memory_usage(), 0);
    EXPECT_EQ(filter.memory_usage() % sizeof(uint64_t), 0);
}

// Test multiple items
TEST_F(BloomfilterTest, MultipleItems) {
    Bloomfilter filter(100, default_error_rate);
    
    std::vector<std::string> items = {"a", "b", "c", "test", "hello"};
    for (const auto& item : items) {
        filter.insert(item);
    }

    // All inserted items should be present
    for (const auto& item : items) {
        EXPECT_TRUE(filter.contains(item));
    }
}

// Test empty string handling
TEST_F(BloomfilterTest, EmptyString) {
    Bloomfilter filter(default_capacity, default_error_rate);
    
    filter.insert("");
    EXPECT_TRUE(filter.contains(""));
}