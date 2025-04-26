#include <core/sharedptr.h>
#include <gtest/gtest.h>
#include <thread>

using namespace core;

struct TestObject {
    static std::atomic<int> destructorCount;
    ~TestObject() { destructorCount++; }
};

std::atomic<int> TestObject::destructorCount = 0;

namespace {

TEST(SharedPtrTest, DefaultConstructor) {
    core::SharedPtr<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr, nullptr);
    EXPECT_EQ(nullptr, ptr);
}

TEST(SharedPtrTest, ConstructWithPointer) {
    int* rawPtr = new int(42);
    core::SharedPtr<int> ptr(rawPtr);
    EXPECT_EQ(ptr.Get(), rawPtr);
    EXPECT_TRUE(ptr);
    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(nullptr, ptr);
}

TEST(SharedPtrTest, CopyConstructor) {
    TestObject::destructorCount = 0;
    {
        core::SharedPtr<TestObject> ptr1(new TestObject());
        {
            core::SharedPtr<TestObject> ptr2(ptr1);
            EXPECT_EQ(TestObject::destructorCount, 0);
        }
        EXPECT_EQ(TestObject::destructorCount, 0);
    }
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrTest, CopyAssignment) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> ptr1(new TestObject());
    core::SharedPtr<TestObject> ptr2;
    ptr2 = ptr1;
    EXPECT_EQ(TestObject::destructorCount, 0);
    ptr1 = core::SharedPtr<TestObject>();  // Release ptr1's reference
    EXPECT_EQ(TestObject::destructorCount, 0);
    ptr2 = core::SharedPtr<TestObject>();  // Release ptr2's reference
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrTest, MoveConstructor) {
    core::SharedPtr<int> ptr1(new int(42));
    core::SharedPtr<int> ptr2(std::move(ptr1));
    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(*ptr2, 42);
}

TEST(SharedPtrTest, MoveAssignment) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> ptr1(new TestObject());
    core::SharedPtr<TestObject> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_NE(ptr2.Get(), nullptr);
    ptr2 = core::SharedPtr<TestObject>();  // Release
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrTest, DereferenceOperators) {
    core::SharedPtr<std::string> ptr(new std::string("test"));
    EXPECT_EQ(*ptr, "test");
    EXPECT_EQ(ptr->size(), 4);
}

TEST(SharedPtrTest, BoolConversion) {
    core::SharedPtr<int> validPtr(new int(5));
    core::SharedPtr<int> nullPtr;
    EXPECT_TRUE(validPtr);
    EXPECT_FALSE(nullPtr);
}

TEST(SharedPtrTest, ComparisonOperators) {
    core::SharedPtr<int> ptr1(new int(5));
    core::SharedPtr<int> ptr2;
    EXPECT_NE(ptr1, nullptr);
    EXPECT_EQ(ptr2, nullptr);
    EXPECT_EQ(nullptr, ptr2);
    EXPECT_NE(nullptr, ptr1);
}

TEST(SharedPtrTest, MakeShared) {
    auto ptr = core::MakeShared<int>(42);
    ASSERT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(SharedPtrTest, SelfAssignment) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> ptr(new TestObject());
    ptr = ptr;                            // Self-assignment, should be no-op
    ptr = core::SharedPtr<TestObject>();  // Release
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrIntegrationTest, MultipleOwners) {
    TestObject::destructorCount = 0;
    {
        core::SharedPtr<TestObject> ptr1 = core::MakeShared<TestObject>();
        {
            core::SharedPtr<TestObject> ptr2 = ptr1;
            {
                core::SharedPtr<TestObject> ptr3 = ptr1;
                EXPECT_EQ(TestObject::destructorCount, 0);
            }
            EXPECT_EQ(TestObject::destructorCount, 0);
        }
        EXPECT_EQ(TestObject::destructorCount, 0);
    }
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrIntegrationTest, AssignmentReleasesOldPointer) {
    TestObject::destructorCount = 0;
    {
        core::SharedPtr<TestObject> ptr1(new TestObject());
        {
            core::SharedPtr<TestObject> ptr2(new TestObject());
            ptr1 = ptr2;
            EXPECT_EQ(TestObject::destructorCount, 1);  // First object deleted
        }  // ptr2 destroyed, count decremented to 1
        EXPECT_EQ(TestObject::destructorCount, 1);
    }  // ptr1 destroyed, count decremented to 0, second object deleted
    EXPECT_EQ(TestObject::destructorCount, 2);
}

TEST(SharedPtrThreadTest, ThreadSafety) {
    TestObject::destructorCount = 0;
    {
        core::SharedPtr<TestObject> globalPtr = core::MakeShared<TestObject>();
        std::vector<std::thread> threads;
        const int numThreads = 100;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&globalPtr]() {
                for (int j = 0; j < 1000; ++j) {
                    core::SharedPtr<TestObject> localPtr = globalPtr;
                    core::SharedPtr<TestObject> anotherPtr = localPtr;
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }
        EXPECT_EQ(TestObject::destructorCount, 0);
    }  // globalPtr destroyed here
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrIntegrationTest, ComplexOwnershipTransfer) {
    TestObject::destructorCount = 0;
    {
        core::SharedPtr<TestObject> ptr1 = core::MakeShared<TestObject>();
        core::SharedPtr<TestObject> ptr2;

        // Transfer ownership via move
        ptr2 = std::move(ptr1);
        EXPECT_EQ(ptr1.Get(), nullptr);
        EXPECT_NE(ptr2.Get(), nullptr);

        // Create a copy chain
        core::SharedPtr<TestObject> ptr3 = ptr2;
        core::SharedPtr<TestObject> ptr4 = ptr3;
        EXPECT_EQ(TestObject::destructorCount, 0);
    }  // All pointers go out of scope
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrIntegrationTest, ObjectLifetimeInNestedScopes) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> outerPtr;

    {
        core::SharedPtr<TestObject> innerPtr = core::MakeShared<TestObject>();
        {
            core::SharedPtr<TestObject> anotherInnerPtr = innerPtr;
            outerPtr = anotherInnerPtr;  // Copy to outer scope
            EXPECT_EQ(TestObject::destructorCount, 0);
        }
        EXPECT_EQ(TestObject::destructorCount, 0);
    }

    EXPECT_EQ(TestObject::destructorCount, 0);  // outerPtr still holds reference
    outerPtr = nullptr;                         // Explicitly release
    EXPECT_EQ(TestObject::destructorCount, 1);
}

TEST(SharedPtrIntegrationTest, UseInContainers) {
    TestObject::destructorCount = 0;
    std::vector<core::SharedPtr<TestObject>> container;

    {
        core::SharedPtr<TestObject> ptr = core::MakeShared<TestObject>();
        container.push_back(ptr);                             // Copy into vector
        container.push_back(core::MakeShared<TestObject>());  // Move into vector
        EXPECT_EQ(TestObject::destructorCount, 0);
    }

    EXPECT_EQ(TestObject::destructorCount, 0);  // Container still holds references
    container.clear();                          // Release all references
    EXPECT_EQ(TestObject::destructorCount, 2);
}

TEST(SharedPtrThreadTest, ConcurrentMixedOperations) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> globalPtr = core::MakeShared<TestObject>();

    auto worker = [&globalPtr](int iterations) {
        for (int i = 0; i < iterations; ++i) {
            // Mix copy, move, and new allocations
            core::SharedPtr<TestObject> localCopy = globalPtr;
            core::SharedPtr<TestObject> movedPtr = std::move(localCopy);
            core::SharedPtr<TestObject> newPtr = core::MakeShared<TestObject>();
            static_cast<void>(movedPtr);  // Prevent optimization
        }
    };

    std::vector<std::thread> threads;
    const int numThreads = 10;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker, 1000);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify global object still exists
    EXPECT_EQ(TestObject::destructorCount, numThreads * 1000);  // All temporary objects destroyed
    globalPtr = nullptr;                                        // Delete global object
    EXPECT_EQ(TestObject::destructorCount, numThreads * 1000 + 1);
}

TEST(SharedPtrEdgeCaseTest, NullptrOperations) {
    core::SharedPtr<int> ptr1;
    core::SharedPtr<int> ptr2 = ptr1;             // Copy null pointer
    core::SharedPtr<int> ptr3 = std::move(ptr1);  // Move null pointer
    ptr3 = ptr2;                                  // Assign null pointer

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr2.Get(), nullptr);
    EXPECT_EQ(ptr3.Get(), nullptr);
}

TEST(SharedPtrEdgeCaseTest, ObjectReassignment) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> ptr;

    ptr = core::MakeShared<TestObject>();  // First object
    ptr = core::MakeShared<TestObject>();  // Second object (first gets destroyed)
    EXPECT_EQ(TestObject::destructorCount, 1);

    ptr = nullptr;  // Second object destroyed
    EXPECT_EQ(TestObject::destructorCount, 2);
}

TEST(SharedPtrIntegrationTest, MixedOperations) {
    TestObject::destructorCount = 0;
    core::SharedPtr<TestObject> globalPtr;

    {
        // Create first object (refcount 1)
        core::SharedPtr<TestObject> ptr1 = core::MakeShared<TestObject>();

        // Copy to globalPtr (refcount 2)
        globalPtr = ptr1;
        EXPECT_EQ(TestObject::destructorCount, 0);

        {  // Nested scope for ptr2
            // Move construction (refcount stays 2)
            core::SharedPtr<TestObject> ptr2(std::move(ptr1));
            EXPECT_EQ(globalPtr.Get(), ptr2.Get());

            // Assign new object to ptr2:
            // 1. Decrements original object's refcount (2 → 1)
            // 2. Creates new object (refcount 1)
            ptr2 = core::MakeShared<TestObject>();
            EXPECT_EQ(TestObject::destructorCount, 0)
                << "Original object still referenced by globalPtr, new object still alive";
        }  // ptr2 (new object) destroyed: new object's refcount 1 → 0 (destructorCount 1)

        EXPECT_EQ(TestObject::destructorCount, 1) << "Temporary object should be destroyed when ptr2 goes out of scope";

    }  // ptr1 destroyed (was already moved-from, no-op)

    // globalPtr still holds original object (refcount 1 → 0 when destroyed)
    EXPECT_EQ(TestObject::destructorCount, 1) << "Original object still alive via globalPtr";

    globalPtr = nullptr;  // Explicitly release
    EXPECT_EQ(TestObject::destructorCount, 2) << "Original object destroyed when last reference released";
}

}  // namespace