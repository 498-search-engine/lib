// d-ary heap tests

#include <core/dary_heap.h>
#include <core/serializer.h>
#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>

using namespace core;

struct VariableSizeStruct {
    size_t hash;
    size_t random_number;
    char* data;
};

class RandomSerializer {
public:
    // Round up to the nearest multiple of 'boundary' (for alignment)
    size_t RoundUp(size_t length, size_t boundary) const {
        static const size_t oneless = boundary - 1;
        return (length + oneless) & ~oneless;
    }

    // Compute how many bytes are needed to serialize the struct properly
    size_t bytesRequired(size_t data_size) const {
        return RoundUp(sizeof(size_t) * 3 + data_size + 1, sizeof(size_t));  // +1 for null terminator
    }

    // Serialize the object into a contiguous byte buffer
    char* serialize(const VariableSizeStruct& obj) const {
        if (obj.data == nullptr) {
            std::cerr << "Serializing Variable Object with Nullptr" << std::endl;
            exit(1);
        }

        size_t data_length = strlen(obj.data) + 1;  // Include null terminator
        size_t buffer_size = bytesRequired(data_length);

        char* buffer = new char[buffer_size];  // Allocate a buffer
        char* ptr = buffer;

        // Store the data length at the start
        std::memcpy(ptr, &data_length, sizeof(size_t));
        ptr += sizeof(size_t);

        // Copy hash
        std::memcpy(ptr, &obj.hash, sizeof(size_t));
        ptr += sizeof(size_t);

        // Copy random_number
        std::memcpy(ptr, &obj.random_number, sizeof(size_t));
        ptr += sizeof(size_t);

        // Copy data (null-terminated string)
        std::memcpy(ptr, obj.data, data_length);
        ptr += RoundUp(data_length, sizeof(size_t));  // Ensure alignment

        return buffer;  // Caller must free the buffer
    }

    // Deserialize from a byte buffer into a VariableSizeStruct object
    VariableSizeStruct deserialize(const char* buffer, size_t size) const {
        VariableSizeStruct obj;
        const char* ptr = buffer;

        // Read data length
        size_t data_length;
        std::memcpy(&data_length, ptr, sizeof(size_t));
        ptr += sizeof(size_t);

        // Read hash
        std::memcpy(&obj.hash, ptr, sizeof(size_t));
        ptr += sizeof(size_t);

        // Read random_number
        std::memcpy(&obj.random_number, ptr, sizeof(size_t));
        ptr += sizeof(size_t);

        // Read data (allocate memory for it)
        obj.data = static_cast<char*>(malloc(data_length));
        if (!obj.data) {
            std::cerr << "Memory allocation failed for data string" << std::endl;
            exit(1);
        }
        std::memcpy(obj.data, ptr, data_length);

        return obj;  // Return the deserialized struct
    }
};


// ✅ Test Serialization of a Valid Struct
TEST(RandomSerializerTest, SerializeDeserialize) {
    RandomSerializer serializer;
    VariableSizeStruct obj{12345, 67890, "Hello, World!"};

    char* buffer = serializer.serialize(obj);
    size_t buffer_size = serializer.bytesRequired(strlen(obj.data));

    VariableSizeStruct deserialized = serializer.deserialize(buffer, buffer_size);

    EXPECT_EQ(deserialized.hash, obj.hash);
    EXPECT_EQ(deserialized.random_number, obj.random_number);
    EXPECT_STREQ(deserialized.data, obj.data);

    delete[] buffer;
    free((void*)deserialized.data);
}

// ✅ Test if `bytesRequired` Computes Correctly
TEST(RandomSerializerTest, BytesRequired) {
    RandomSerializer serializer;

    size_t expected_size = serializer.RoundUp((sizeof(size_t) * 3) + 14, sizeof(size_t));  // "Hello, World!" + null
    EXPECT_EQ(serializer.bytesRequired(13), expected_size);
}

// ✅ Test Empty String Serialization
TEST(RandomSerializerTest, SerializeEmptyString) {
    RandomSerializer serializer;
    VariableSizeStruct obj{98765, 43210, ""};

    char* buffer = serializer.serialize(obj);
    size_t buffer_size = serializer.bytesRequired(strlen(obj.data));

    VariableSizeStruct deserialized = serializer.deserialize(buffer, buffer_size);

    EXPECT_EQ(deserialized.hash, obj.hash);
    EXPECT_EQ(deserialized.random_number, obj.random_number);
    EXPECT_STREQ(deserialized.data, obj.data);

    delete[] buffer;
    free((void*)deserialized.data);
}

// ✅ Test Long String Serialization
TEST(RandomSerializerTest, SerializeLongString) {
    RandomSerializer serializer;
    std::string long_string(1000, 'A');
    VariableSizeStruct obj{11111, 22222, (char*)long_string.c_str()};

    char* buffer = serializer.serialize(obj);
    size_t buffer_size = serializer.bytesRequired(long_string.size());

    VariableSizeStruct deserialized = serializer.deserialize(buffer, buffer_size);

    EXPECT_EQ(deserialized.hash, obj.hash);
    EXPECT_EQ(deserialized.random_number, obj.random_number);
    EXPECT_STREQ(deserialized.data, obj.data);

    delete[] buffer;
    free((void*)deserialized.data);
}

// ✅ Test Null Pointer Case (shouldn't crash)
TEST(RandomSerializerTest, NullDataHandling) {
    RandomSerializer serializer;
    VariableSizeStruct obj{55555, 66666, nullptr};

    ASSERT_EXIT(serializer.serialize(obj), ::testing::ExitedWithCode(1), ".*");
}
// ✅ Test Alignment (Ensuring RoundUp Works Properly)
TEST(RandomSerializerTest, Alignment) {
    RandomSerializer serializer;

    EXPECT_EQ(serializer.RoundUp(10, 8), 16);
    EXPECT_EQ(serializer.RoundUp(17, 8), 24);
    EXPECT_EQ(serializer.RoundUp(32, 8), 32);
}

// ✅ Test Serialization Buffer Allocation
TEST(RandomSerializerTest, BufferAllocation) {
    RandomSerializer serializer;
    VariableSizeStruct obj{123, 456, "Test String"};

    char* buffer = serializer.serialize(obj);
    EXPECT_NE(buffer, nullptr);

    delete[] buffer;
}

// ✅ Test Deserialization from Invalid Buffer
// TEST(RandomSerializerTest, DeserializeInvalidBuffer) {
//     RandomSerializer serializer;
//     char buffer[32] = {};

//     EXPECT_THROW(serializer.deserialize(buffer, sizeof(buffer)), std::runtime_error);
// }

// ✅ Test Deserializing a Struct with Uninitialized Data
// TEST(RandomSerializerTest, DeserializeUninitializedData) {
//     RandomSerializer serializer;
//     char raw_data[32] = {};

//     VariableSizeStruct obj = serializer.deserialize(raw_data, sizeof(raw_data));

//     EXPECT_EQ(obj.hash, 0);
//     EXPECT_EQ(obj.random_number, 0);
//     EXPECT_STREQ(obj.data, "");
// }