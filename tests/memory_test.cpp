// tests for memory (unique_ptr for now)
// authors: @Anubhav652

#include <core/memory.h>
#include <gtest/gtest.h>

using namespace core;

class TestObject {
public:
    TestObject(int value) : value_(value) {}
    int GetValue() const { return value_; }

private:
    int value_;
};

// Test for UniquePtr
TEST(UniquePtrTest, BasicOperations) {
    UniquePtr<int> ptr(new int(42));
    EXPECT_EQ(*ptr, 42);
    int* rawPtr = ptr.Release();
    EXPECT_EQ(*rawPtr, 42);
    delete rawPtr;

    ptr.Reset(new int(100));
    EXPECT_EQ(*ptr, 100);
}

// Test 1: Default Initialization
TEST(UniquePtrTest, DefaultInitialization) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
}

// Test 2: Basic Dereferencing
TEST(UniquePtrTest, Dereferencing) {
    UniquePtr<int> ptr(new int(42));
    EXPECT_EQ(*ptr, 42);
    EXPECT_NE(ptr.Get(), nullptr);
}

// Test 3: Ownership Transfer (Move Constructor)
TEST(UniquePtrTest, MoveConstructor) {
    UniquePtr<int> ptr1(new int(10));
    UniquePtr<int> ptr2(std::move(ptr1));
    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(*ptr2, 10);
}

// Test 4: Ownership Transfer (Move Assignment)
TEST(UniquePtrTest, MoveAssignment) {
    UniquePtr<int> ptr1(new int(20));
    UniquePtr<int> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(*ptr2, 20);
}

// Test 5: Release
TEST(UniquePtrTest, Release) {
    UniquePtr<int> ptr(new int(30));
    int* rawPtr = ptr.Release();
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(*rawPtr, 30);
    delete rawPtr;
}

// Test 6: Reset
TEST(UniquePtrTest, Reset) {
    UniquePtr<int> ptr(new int(40));
    ptr.Reset(new int(50));
    EXPECT_EQ(*ptr, 50);
}

// Test 7: Swap
TEST(UniquePtrTest, Swap) {
    UniquePtr<int> ptr1(new int(60));
    UniquePtr<int> ptr2(new int(70));
    ptr1.Swap(ptr2);
    EXPECT_EQ(*ptr1, 70);
    EXPECT_EQ(*ptr2, 60);
}

// Test 8: Custom Object
TEST(UniquePtrTest, CustomObject) {
    UniquePtr<TestObject> obj(new TestObject(100));
    EXPECT_EQ(obj->GetValue(), 100);
}