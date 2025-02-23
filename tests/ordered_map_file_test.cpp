#include "core/ordered_map_file.h"

#include <filesystem>
#include <gtest/gtest.h>

using namespace core;

namespace {

constexpr const char* OrderedMapFileName = "ordered_map.dat";

class OrderedMapFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure test file doesn't exist before each test
        std::filesystem::remove(OrderedMapFileName);
    }

    void TearDown() override {
        // Clean up test file after each test
        std::filesystem::remove(OrderedMapFileName);
    }
};

}  // namespace

struct U32Compare {
    int operator()(uint32_t a, auto b) const {
        if (a < b) {
            return -1;
        } else if (a > b) {
            return 1;
        } else {
            return 0;
        }
    }
};

TEST_F(OrderedMapFileTest, EmptyTreeContainsFalse) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};
    EXPECT_FALSE(tree.Contains(1));
    EXPECT_TRUE(tree.Empty());
}

TEST_F(OrderedMapFileTest, InsertAndRead) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    EXPECT_TRUE(tree.Insert(1, 100));
    EXPECT_TRUE(tree.Contains(1));
    EXPECT_FALSE(tree.Contains(2));

    EXPECT_FALSE(tree.Empty());
    EXPECT_EQ(tree.Size(), 1);

    auto find = tree.Find(1);
    ASSERT_TRUE(find.HasValue());
    EXPECT_EQ(*find->key, 1);
    EXPECT_EQ(*find->value, 100);
}

TEST_F(OrderedMapFileTest, Persistence) {
    // Write some data
    {
        auto compare = U32Compare{};
        auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

        tree.Insert(1, 10);
        tree.Insert(2, 20);
        tree.Insert(3, 30);
    }

    // Read it back in a new instance
    {
        auto compare = U32Compare{};
        auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

        EXPECT_EQ(tree.Size(), 3);
        ASSERT_TRUE(tree.Contains(1));
        ASSERT_TRUE(tree.Contains(2));
        ASSERT_TRUE(tree.Contains(3));
        EXPECT_EQ(*tree.Find(1)->value, 10);
        EXPECT_EQ(*tree.Find(2)->value, 20);
        EXPECT_EQ(*tree.Find(3)->value, 30);
    }
}

TEST_F(OrderedMapFileTest, DuplicateInsert) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    EXPECT_TRUE(tree.Insert(1, 100));
    EXPECT_FALSE(tree.Insert(1, 200));  // Duplicate should fail
    EXPECT_TRUE(tree.Contains(1));
    EXPECT_EQ(tree.Size(), 1);
}

TEST_F(OrderedMapFileTest, MultipleInserts) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    for (uint32_t i = 0; i < 10; i++) {
        EXPECT_TRUE(tree.Insert(i, i * 100));
    }

    for (uint32_t i = 0; i < 10; i++) {
        EXPECT_TRUE(tree.Contains(i)) << "Failed to find key " << i;
    }
}

TEST_F(OrderedMapFileTest, ForceSplit) {
    auto compare = U32Compare{};
    constexpr auto NodeKeys = 10;
    auto tree = OrderedMapFile<uint32_t, uint32_t, NodeKeys, decltype(compare)>{OrderedMapFileName, compare};

    // Insert enough elements to force at least one split
    for (uint32_t i = 0; i < NodeKeys + 1; i++) {
        EXPECT_TRUE(tree.Insert(i, i * 100));
    }

    // Verify all elements are still findable
    for (uint32_t i = 0; i < NodeKeys + 1; i++) {
        EXPECT_TRUE(tree.Contains(i));
    }
}

TEST_F(OrderedMapFileTest, ReverseInsertOrder) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    for (uint32_t i = 10; i > 0; i--) {
        EXPECT_TRUE(tree.Insert(i, i * 100)) << "Failed to insert " << i;
        for (uint32_t j = 10; j >= i; --j) {
            EXPECT_TRUE(tree.Contains(j)) << "Failed to find " << j << " after inserting " << i;
        }
    }

    for (uint32_t i = 1; i <= 10; i++) {
        EXPECT_TRUE(tree.Contains(i)) << "Failed to find " << i;
    }
    EXPECT_FALSE(tree.Contains(0));
    EXPECT_FALSE(tree.Contains(101));
}

TEST_F(OrderedMapFileTest, AlternatingInsertPattern) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    // Insert in pattern: 50, 0, 49, 1, 48, 2, ...
    for (uint32_t i = 0; i < 25; i++) {
        EXPECT_TRUE(tree.Insert(50 - i, (50 - i) * 100));
        EXPECT_TRUE(tree.Insert(i, i * 100));
    }

    // Insert 25
    EXPECT_TRUE(tree.Insert(25, 25 * 100));

    for (uint32_t i = 0; i <= 50; i++) {
        EXPECT_TRUE(tree.Contains(i)) << "Failed to find " << i;
    }
}

TEST_F(OrderedMapFileTest, ContainsWithDifferentTypes) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    EXPECT_TRUE(tree.Insert(42, 4200));

    // Test with different types that should be comparable
    EXPECT_TRUE(tree.Contains(42));    // uint32_t
    EXPECT_TRUE(tree.Contains(42L));   // long
    EXPECT_TRUE(tree.Contains(42.0));  // double

    // These should all not be found
    EXPECT_FALSE(tree.Contains(43));
    EXPECT_FALSE(tree.Contains(43L));
    EXPECT_FALSE(tree.Contains(43.0));
}

TEST_F(OrderedMapFileTest, LargeNumberOfInserts) {
    auto compare = U32Compare{};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 128, decltype(compare)>{OrderedMapFileName, compare};

    const uint32_t n = 100000;

    // Insert n elements
    for (uint32_t i = 0; i < n; i++) {
        EXPECT_TRUE(tree.Insert(i, i * 2));
    }

    EXPECT_EQ(tree.Size(), n);

    // Verify all elements
    for (uint32_t i = 0; i < n; i++) {
        auto it = tree.Find(i);
        ASSERT_TRUE(it.HasValue()) << "Failed to find key " << i;
        EXPECT_EQ(*it->key, i);
        EXPECT_EQ(*it->value, i * 2);
    }

    // Verify elements outside range don't exist
    EXPECT_FALSE(tree.Contains(n));
    EXPECT_FALSE(tree.Contains(n + 1));
    EXPECT_FALSE(tree.Find(n).HasValue());
    EXPECT_FALSE(tree.Find(n + 1).HasValue());
}

TEST_F(OrderedMapFileTest, InsertAfterSplit) {
    auto compare = U32Compare{};
    constexpr auto NodeKeys = 10;
    auto tree = OrderedMapFile<uint32_t, uint32_t, NodeKeys, decltype(compare)>{OrderedMapFileName, compare};

    // Fill first node
    for (uint32_t i = 0; i < NodeKeys; i++) {
        EXPECT_TRUE(tree.Insert(i * 2, i * 100));
    }

    // This should cause a split
    EXPECT_TRUE(tree.Insert(NodeKeys * 2, 9999));

    // Insert between existing values
    EXPECT_TRUE(tree.Insert(3, 333));
    EXPECT_TRUE(tree.Contains(3));

    // Verify all original values still exist
    for (uint32_t i = 0; i < NodeKeys; i++) {
        EXPECT_TRUE(tree.Contains(i * 2));
    }
    EXPECT_TRUE(tree.Contains(NodeKeys * 2));
}

TEST_F(OrderedMapFileTest, HybridStringContains) {
    std::vector<std::string> strings;

    struct HybridCompare {
        std::vector<std::string>& strings;
        auto operator()(uint32_t a, uint32_t b) const { return strings[a].compare(strings[b]); }
        auto operator()(uint32_t a, std::string_view b) const { return strings[a].compare(b); }
    };

    auto compare = HybridCompare{strings};
    auto tree = OrderedMapFile<uint32_t, uint32_t, 5, decltype(compare)>{OrderedMapFileName, compare};

    for (uint32_t i = 1; i <= 20; ++i) {
        strings.push_back(std::string{"hello "} + std::to_string(i));
        tree.Insert(strings.size() - 1, i);
    }

    for (uint32_t i = 1; i <= 20; ++i) {
        auto s = std::string{"hello "} + std::to_string(i);
        EXPECT_TRUE(tree.Contains(s));
    }

    for (uint32_t i = 21; i <= 41; ++i) {
        auto s = std::string{"hello "} + std::to_string(i);
        EXPECT_FALSE(tree.Contains(s));
    }
}

struct FixedSizeRecord {
    int id;
    char description[100];  // Fixed-size char array
};

// Ensure the struct is trivially copyable
static_assert(std::is_trivially_copyable_v<FixedSizeRecord>, "AnotherFixedSizeRecord must be trivially copyable");

struct IntComparator {
    int operator()(int a, int b) const {
        return (a < b) ? -1 : (a > b) ? 1 : 0;
    }
};

TEST_F(OrderedMapFileTest, AnotherFixedSizeRecordStorage) {
    OrderedMapFile<int, FixedSizeRecord, 4, IntComparator> map(OrderedMapFileName, IntComparator{});

    FixedSizeRecord record1 = {1, "Description One"};
    FixedSizeRecord record2 = {2, "Description Two"};

    EXPECT_TRUE(map.Insert(1, record1));
    EXPECT_TRUE(map.Insert(2, record2));

    auto found1 = map.Find(1);
    ASSERT_TRUE(found1.HasValue());
    EXPECT_EQ(found1->value->id, 1);
    EXPECT_STREQ(found1->value->description, "Description One");

    auto found2 = map.Find(2);
    ASSERT_TRUE(found2.HasValue());
    EXPECT_EQ(found2->value->id, 2);
    EXPECT_STREQ(found2->value->description, "Description Two");
}

TEST_F(OrderedMapFileTest, AnotherFixedSizeRecordPersistence) {
    // Write some records
    {
        OrderedMapFile<int, FixedSizeRecord, 4, IntComparator> map(OrderedMapFileName, IntComparator{});
        FixedSizeRecord record1 = {1, "Persistent Description One"};
        FixedSizeRecord record2 = {2, "Persistent Description Two"};
        map.Insert(1, record1);
        map.Insert(2, record2);
    }

    // Read them back in a new instance
    {
        OrderedMapFile<int, FixedSizeRecord, 4, IntComparator> map(OrderedMapFileName, IntComparator{});
        auto found1 = map.Find(1);
        ASSERT_TRUE(found1.HasValue());
        EXPECT_EQ(found1->value->id, 1);
        EXPECT_STREQ(found1->value->description, "Persistent Description One");

        auto found2 = map.Find(2);
        ASSERT_TRUE(found2.HasValue());
        EXPECT_EQ(found2->value->id, 2);
        EXPECT_STREQ(found2->value->description, "Persistent Description Two");
    }
}
