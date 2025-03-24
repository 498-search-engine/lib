#include "core/lru_cache.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>

using namespace core;

// For testing with complex objects and move semantics
class ComplexObject {
public:
    ComplexObject(int val, std::string str) : value(val), data(std::move(str)), ptr(std::make_unique<int>(val)) {}

    ComplexObject(const ComplexObject& other) = delete;
    ComplexObject& operator=(const ComplexObject& other) = delete;

    ComplexObject(ComplexObject&& other) noexcept = default;
    ComplexObject& operator=(ComplexObject&& other) noexcept = default;

    bool operator==(const ComplexObject& other) const {
        return value == other.value && data == other.data &&
               (ptr == other.ptr || (ptr && other.ptr && *ptr == *other.ptr));
    }

    int value;
    std::string data;
    std::unique_ptr<int> ptr;
};

// Basic LRUCache Tests for int keys and values
class LRUCacheIntTest : public ::testing::Test {
protected:
    void SetUp() override { cache = std::make_unique<LRUCache<int, int>>(3); }

    std::unique_ptr<LRUCache<int, int>> cache;
};

// Tests with string keys and values
class LRUCacheStringTest : public ::testing::Test {
protected:
    void SetUp() override { cache = std::make_unique<LRUCache<std::string, std::string>>(3); }

    std::unique_ptr<LRUCache<std::string, std::string>> cache;
};

// Tests with complex objects
class LRUCacheComplexTest : public ::testing::Test {
protected:
    void SetUp() override { cache = std::make_unique<LRUCache<int, ComplexObject>>(3); }

    std::unique_ptr<LRUCache<int, ComplexObject>> cache;
};

// Basic Functionality Tests
TEST_F(LRUCacheIntTest, InitialSizeIsZero) {
    EXPECT_EQ(cache->Size(), 0);
}

TEST_F(LRUCacheIntTest, EmptyCacheFindReturnsNull) {
    EXPECT_EQ(cache->Find(1), nullptr);
}

// Insertion Tests
TEST_F(LRUCacheIntTest, InsertIntoEmptyCache) {
    auto [ptr, inserted] = cache->Insert({1, 100});

    EXPECT_TRUE(inserted);
    EXPECT_EQ(ptr->first, 1);
    EXPECT_EQ(ptr->second, 100);
    EXPECT_EQ(cache->Size(), 1);
}

TEST_F(LRUCacheIntTest, InsertMultipleItems) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});

    EXPECT_EQ(cache->Size(), 2);

    auto* item1 = cache->Find(1);
    auto* item2 = cache->Find(2);

    EXPECT_NE(item1, nullptr);
    EXPECT_NE(item2, nullptr);
    EXPECT_EQ(item1->second, 100);
    EXPECT_EQ(item2->second, 200);
}

TEST_F(LRUCacheIntTest, InsertDuplicateKey) {
    cache->Insert({1, 100});
    auto [ptr, inserted] = cache->Insert({1, 200});

    EXPECT_FALSE(inserted);
    EXPECT_EQ(ptr->second, 100);  // Value should not change
    EXPECT_EQ(cache->Size(), 1);
}

TEST_F(LRUCacheIntTest, InsertWithEviction) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Cache is now full
    EXPECT_EQ(cache->Size(), 3);

    // Insert a new item, should evict the least recently used (1)
    cache->Insert({4, 400});

    EXPECT_EQ(cache->Size(), 3);
    EXPECT_EQ(cache->Find(1), nullptr);  // Key 1 should be evicted
    EXPECT_NE(cache->Find(2), nullptr);
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
}

// Lookup Tests
TEST_F(LRUCacheIntTest, FindExistingItem) {
    cache->Insert({1, 100});

    auto* item = cache->Find(1);
    EXPECT_NE(item, nullptr);
    EXPECT_EQ(item->first, 1);
    EXPECT_EQ(item->second, 100);
}

TEST_F(LRUCacheIntTest, ConstFindExistingItem) {
    cache->Insert({1, 100});

    const auto& constCache = *cache;
    const auto* item = constCache.Find(1);
    EXPECT_NE(item, nullptr);
    EXPECT_EQ(item->first, 1);
    EXPECT_EQ(item->second, 100);
}

TEST_F(LRUCacheIntTest, FindUpdatesLRUOrder) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Access key 1, making it the most recently used
    cache->Find(1);

    // Insert a new item, should evict key 2 (now the least recently used)
    cache->Insert({4, 400});

    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_EQ(cache->Find(2), nullptr);  // Key 2 should be evicted
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
}

TEST_F(LRUCacheIntTest, OperatorBracketAccess) {
    (*cache)[1] = 100;

    EXPECT_EQ(cache->Size(), 1);
    EXPECT_EQ((*cache)[1], 100);
}

TEST_F(LRUCacheIntTest, OperatorBracketInsert) {
    // This should insert a default-constructed int (0)
    int val = (*cache)[1];

    EXPECT_EQ(val, 0);
    EXPECT_EQ(cache->Size(), 1);

    // Now modify the value
    (*cache)[1] = 100;
    EXPECT_EQ((*cache)[1], 100);
}

TEST_F(LRUCacheIntTest, OperatorBracketUpdatesLRUOrder) {
    (*cache)[1] = 100;
    (*cache)[2] = 200;
    (*cache)[3] = 300;

    // Access key 1
    int val = (*cache)[1];

    // Insert new item, should evict key 2
    (*cache)[4] = 400;

    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_EQ(cache->Find(2), nullptr);  // Key 2 should be evicted
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
}

// LRU Policy Tests
TEST_F(LRUCacheIntTest, LRUEvictionOrder) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Access items in reverse order to change LRU ordering
    cache->Find(2);
    cache->Find(1);

    // Insert new item, should evict key 3 (least recently used)
    cache->Insert({4, 400});

    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(2), nullptr);
    EXPECT_EQ(cache->Find(3), nullptr);  // Key 3 should be evicted
    EXPECT_NE(cache->Find(4), nullptr);
}

TEST_F(LRUCacheIntTest, ComplexEvictionSequence) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Access key 1
    cache->Find(1);

    // 1 3 2

    // Insert key 4, evicting key 2
    cache->Insert({4, 400});

    // 4 1 3

    // Access key 3
    cache->Find(3);

    // 3 4 1

    // Insert key 5, evicting key 1
    cache->Insert({5, 500});

    EXPECT_EQ(cache->Find(1), nullptr);
    EXPECT_EQ(cache->Find(2), nullptr);
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
    EXPECT_NE(cache->Find(5), nullptr);
}

// String Tests
TEST_F(LRUCacheStringTest, BasicStringOperations) {
    cache->Insert({"key1", "value1"});
    cache->Insert({"key2", "value2"});

    auto* item = cache->Find("key1");
    EXPECT_NE(item, nullptr);
    EXPECT_EQ(item->second, "value1");

    (*cache)["key3"] = "value3";
    EXPECT_EQ((*cache)["key3"], "value3");

    // Insert a fourth item, evicting the least recently used
    cache->Insert({"key4", "value4"});

    EXPECT_NE(cache->Find("key1"), nullptr);
    EXPECT_EQ(cache->Find("key2"), nullptr);  // Should be evicted
    EXPECT_NE(cache->Find("key3"), nullptr);
    EXPECT_NE(cache->Find("key4"), nullptr);
}

// Move Semantics Tests
TEST_F(LRUCacheComplexTest, MoveSemantics) {
    // Test with move-only values
    ComplexObject obj1(1, "test1");
    std::string originalData = obj1.data;

    // Insert with move
    auto [ptr, inserted] = cache->Insert({1, std::move(obj1)});

    EXPECT_TRUE(inserted);
    EXPECT_EQ(ptr->second.value, 1);
    EXPECT_EQ(ptr->second.data, originalData);
    EXPECT_TRUE(obj1.data.empty());  // Original data should be moved

    // Find the object
    auto* item = cache->Find(1);
    EXPECT_NE(item, nullptr);
    EXPECT_EQ(item->second.value, 1);
    EXPECT_EQ(item->second.data, originalData);
}

// Edge Cases
TEST_F(LRUCacheIntTest, CacheSizeOne) {
    LRUCache<int, int> singleCache(1);

    singleCache.Insert({1, 100});
    EXPECT_EQ(singleCache.Size(), 1);

    // Insert a second item, first should be evicted
    singleCache.Insert({2, 200});
    EXPECT_EQ(singleCache.Size(), 1);
    EXPECT_EQ(singleCache.Find(1), nullptr);
    EXPECT_NE(singleCache.Find(2), nullptr);
}

// Performance hints test
TEST_F(LRUCacheIntTest, SequentialAccess) {
    // Fill the cache
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Sequential access shouldn't cause thrashing
    for (int i = 0; i < 10; i++) {
        // Access the items in the same order repeatedly
        cache->Find(1);
        cache->Find(2);
        cache->Find(3);
    }

    // All items should still be in the cache
    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(2), nullptr);
    EXPECT_NE(cache->Find(3), nullptr);
}

// Erase Method Tests
TEST_F(LRUCacheIntTest, EraseExistingItem) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    EXPECT_EQ(cache->Size(), 2);

    // Erase an existing item
    cache->Erase(1);

    EXPECT_EQ(cache->Size(), 1);
    EXPECT_EQ(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(2), nullptr);
}

TEST_F(LRUCacheIntTest, EraseNonExistentItem) {
    cache->Insert({1, 100});
    EXPECT_EQ(cache->Size(), 1);

    // Try to erase a non-existent key
    cache->Erase(2);

    // Cache should remain unchanged
    EXPECT_EQ(cache->Size(), 1);
    EXPECT_NE(cache->Find(1), nullptr);
}

TEST_F(LRUCacheIntTest, EraseAndReinsert) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});

    // Erase item 1
    cache->Erase(1);
    EXPECT_EQ(cache->Size(), 1);

    // Re-insert item 1 with different value
    auto [ptr, inserted] = cache->Insert({1, 150});

    EXPECT_TRUE(inserted);
    EXPECT_EQ(ptr->second, 150);
    EXPECT_EQ(cache->Size(), 2);
}

TEST_F(LRUCacheIntTest, EraseAllItems) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Erase all items
    cache->Erase(1);
    cache->Erase(2);
    cache->Erase(3);

    EXPECT_EQ(cache->Size(), 0);
    EXPECT_EQ(cache->Find(1), nullptr);
    EXPECT_EQ(cache->Find(2), nullptr);
    EXPECT_EQ(cache->Find(3), nullptr);
}

TEST_F(LRUCacheIntTest, EraseAndEviction) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Erase item 2
    cache->Erase(2);
    EXPECT_EQ(cache->Size(), 2);

    // Insert two new items
    cache->Insert({4, 400});
    cache->Insert({5, 500});

    // Cache should now contain items 1, 3, 4, 5 with 1 being the least recent,
    // but since the capacity is 3, item 1 should be evicted
    EXPECT_EQ(cache->Size(), 3);
    EXPECT_EQ(cache->Find(1), nullptr);  // Evicted
    EXPECT_EQ(cache->Find(2), nullptr);  // Erased
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
    EXPECT_NE(cache->Find(5), nullptr);
}

TEST_F(LRUCacheIntTest, EraseAfterFind) {
    cache->Insert({1, 100});
    cache->Insert({2, 200});
    cache->Insert({3, 300});

    // Access item 1, making it the most recently used
    cache->Find(1);

    // 1 3 2

    // Erase item 2
    cache->Erase(2);

    // 1 3

    // Insert new item
    cache->Insert({4, 400});

    // 4 1 3

    EXPECT_EQ(cache->Size(), 3);
    EXPECT_EQ(cache->Find(2), nullptr);  // Erased
    EXPECT_NE(cache->Find(3), nullptr);
    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);

    cache->Insert({5, 500});

    // 5 4 1

    EXPECT_EQ(cache->Size(), 3);
    EXPECT_EQ(cache->Find(2), nullptr);  // Erased
    EXPECT_EQ(cache->Find(3), nullptr);  // Evicted
    EXPECT_NE(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(4), nullptr);
    EXPECT_NE(cache->Find(5), nullptr);
}

TEST_F(LRUCacheStringTest, EraseStringKeys) {
    cache->Insert({"key1", "value1"});
    cache->Insert({"key2", "value2"});

    // Erase by string key
    cache->Erase("key1");

    EXPECT_EQ(cache->Size(), 1);
    EXPECT_EQ(cache->Find("key1"), nullptr);
    EXPECT_NE(cache->Find("key2"), nullptr);
}

TEST_F(LRUCacheComplexTest, EraseWithComplexValues) {
    cache->Insert({1, ComplexObject(1, "test1")});
    cache->Insert({2, ComplexObject(2, "test2")});

    // Verify objects exist
    auto* item1 = cache->Find(1);
    auto* item2 = cache->Find(2);
    EXPECT_NE(item1, nullptr);
    EXPECT_NE(item2, nullptr);

    // Erase first item
    cache->Erase(1);

    // Check state
    EXPECT_EQ(cache->Size(), 1);
    EXPECT_EQ(cache->Find(1), nullptr);
    EXPECT_NE(cache->Find(2), nullptr);
}

TEST_F(LRUCacheIntTest, EraseAndOperatorBracket) {
    (*cache)[1] = 100;
    (*cache)[2] = 200;

    // Erase item 1
    cache->Erase(1);

    // Access via operator[] should re-insert a default-constructed value
    int val = (*cache)[1];

    EXPECT_EQ(val, 0);  // Default int value
    EXPECT_EQ(cache->Size(), 2);

    // Modify the value
    (*cache)[1] = 150;
    EXPECT_EQ((*cache)[1], 150);
}
