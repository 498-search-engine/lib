#include "core/vector_file.h"

#include <filesystem>
#include <gtest/gtest.h>

using namespace core;

namespace {

constexpr const char* VectorFileName = "vector_file.dat";

class VectorFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure test file doesn't exist before each test
        std::filesystem::remove(VectorFileName);
    }

    void TearDown() override {
        // Clean up test file after each test
        std::filesystem::remove(VectorFileName);
    }
};

}  // namespace

// Test struct to verify trivially copyable check
struct TrivialStruct {
    int x;
    double y;
    char z;
};

struct FixedSizeRecord {
    int id;
    char name[100];  // Fixed-size char array
};

// Ensure the struct is trivially copyable
static_assert(std::is_trivially_copyable_v<FixedSizeRecord>, "FixedSizeRecord must be trivially copyable");

TEST_F(VectorFileTest, CreateNewFile) {
    VectorFile<int> list(VectorFileName);
    EXPECT_EQ(list.Size(), 0);
    EXPECT_GT(list.Capacity(), 0);
}

TEST_F(VectorFileTest, PushBackAndAccess) {
    VectorFile<int> list(VectorFileName);

    list.PushBack(42);
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list[0], 42);

    list.PushBack(123);
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list[0], 42);
    EXPECT_EQ(list[1], 123);
}

TEST_F(VectorFileTest, PopBack) {
    VectorFile<int> list(VectorFileName);

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    ASSERT_EQ(list.Size(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);

    list.PopBack();
    ASSERT_EQ(list.Size(), 2);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);

    list.PopBack();
    ASSERT_EQ(list.Size(), 1);
    EXPECT_EQ(list[0], 1);

    list.PopBack();
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.Empty());
}

TEST_F(VectorFileTest, Persistence) {
    // Write some data
    {
        VectorFile<int> list(VectorFileName);
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);
    }

    // Read it back in a new instance
    {
        VectorFile<int> list(VectorFileName);
        EXPECT_EQ(list.Size(), 3);
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[1], 2);
        EXPECT_EQ(list[2], 3);
    }
}

TEST_F(VectorFileTest, Grow) {
    VectorFile<int> list(VectorFileName);
    size_t initial_capacity = list.Capacity();

    // Add enough elements to force growth
    for (int i = 0; i < initial_capacity + 1; i++) {
        list.PushBack(i);
    }

    EXPECT_GT(list.Capacity(), initial_capacity);

    // Verify all elements are intact
    for (size_t i = 0; i < list.Size(); i++) {
        EXPECT_EQ(list[i], i);
    }
}

TEST_F(VectorFileTest, Shrink) {
    VectorFile<int> list(VectorFileName);

    // Add many elements to force growth
    for (int i = 0; i < 5 * VectorFile<int>::EntriesPerPage; i++) {
        list.PushBack(i);
    }

    size_t peak_capacity = list.Capacity();

    // Remove most elements to trigger shrink
    while (list.Size() > VectorFile<int>::EntriesPerPage) {
        list.PopBack();
        ASSERT_LE(list.Size(), list.Capacity());
    }

    EXPECT_LT(list.Capacity(), peak_capacity);
}

TEST_F(VectorFileTest, OutOfBoundsAccess) {
    VectorFile<int> list(VectorFileName);
    list.PushBack(1);

    EXPECT_THROW(list[1], std::out_of_range);
    list.PopBack();
    EXPECT_THROW(list.PopBack(), std::out_of_range);
}

TEST_F(VectorFileTest, StructStorage) {
    VectorFile<TrivialStruct> list(VectorFileName);

    TrivialStruct s1{.x = 1, .y = 2.0, .z = 'a'};
    TrivialStruct s2{.x = 2, .y = 3.0, .z = 'b'};

    list.PushBack(s1);
    list.PushBack(s2);

    EXPECT_EQ(list[0].x, 1);
    EXPECT_EQ(list[0].y, 2.0);
    EXPECT_EQ(list[0].z, 'a');

    EXPECT_EQ(list[1].x, 2);
    EXPECT_EQ(list[1].y, 3.0);
    EXPECT_EQ(list[1].z, 'b');
}

TEST_F(VectorFileTest, LargeFile) {
    VectorFile<int> list(VectorFileName);
    const size_t count = 1000000;  // 1M elements

    // Add many elements
    for (int i = 0; i < count; i++) {
        list.PushBack(i);
    }

    EXPECT_EQ(list.Size(), count);

    // Verify random access still works
    EXPECT_EQ(list[0], 0);
    EXPECT_EQ(list[count / 2], count / 2);
    EXPECT_EQ(list[count - 1], count - 1);
}

TEST_F(VectorFileTest, CustomData) {
    struct SpecialData {
        uint32_t sum;
    };

    {
        CustomVectorFile<int, SpecialData> list(VectorFileName);
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);
        list.CustomData()->sum = 6;
    }

    {
        CustomVectorFile<int, SpecialData> list(VectorFileName);
        ASSERT_EQ(list.Size(), 3);
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[1], 2);
        EXPECT_EQ(list[2], 3);
        EXPECT_EQ(list.CustomData()->sum, 6);
    }
}

TEST_F(VectorFileTest, CustomDataInitialization) {
    struct CustomData {
        int value;
    };

    {
        CustomVectorFile<int, CustomData> list(VectorFileName);
        list.CustomData()->value = 42;
    }

    {
        CustomVectorFile<int, CustomData> list(VectorFileName);
        EXPECT_EQ(list.CustomData()->value, 42);
    }
}

TEST_F(VectorFileTest, FixedSizeRecordStorage) {
    CustomVectorFile<FixedSizeRecord, void> list(VectorFileName);

    FixedSizeRecord record1 = {1, "Record One"};
    FixedSizeRecord record2 = {2, "Record Two"};

    list.PushBack(record1);
    list.PushBack(record2);

    EXPECT_EQ(list.Size(), 2);

    EXPECT_EQ(list[0].id, 1);
    EXPECT_STREQ(list[0].name, "Record One");

    EXPECT_EQ(list[1].id, 2);
    EXPECT_STREQ(list[1].name, "Record Two");
}

TEST_F(VectorFileTest, FixedSizeRecordPersistence) {
    // Write some records
    {
        CustomVectorFile<FixedSizeRecord, void> list(VectorFileName);
        FixedSizeRecord record1 = {1, "Persistent One"};
        FixedSizeRecord record2 = {2, "Persistent Two"};
        list.PushBack(record1);
        list.PushBack(record2);
    }

    // Read them back in a new instance
    {
        CustomVectorFile<FixedSizeRecord, void> list(VectorFileName);
        ASSERT_EQ(list.Size(), 2);

        EXPECT_EQ(list[0].id, 1);
        EXPECT_STREQ(list[0].name, "Persistent One");

        EXPECT_EQ(list[1].id, 2);
        EXPECT_STREQ(list[1].name, "Persistent Two");
    }
}
