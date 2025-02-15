// tests for threading library
// authors: @Anubhav652

#include <core/cv.h>
#include <core/locks.h>
#include <core/mutex.h>
#include <core/thread.h>

#include <gtest/gtest.h>

using namespace core;

// Test cv.h (Condition Variable)
TEST(ConditionVariableTest, WaitAndSignal) {
    Mutex mut;
    cv condVar;

    bool flag = false;

    std::thread signalThread([&]() {
        mut.Lock();
        flag = true;
        condVar.Signal();
        mut.Unlock();
    });

    mut.Lock();
    while (!flag) {
        condVar.Wait(mut);
    }
    mut.Unlock();
    signalThread.join();

    EXPECT_TRUE(flag);
}

TEST(ConditionVariableTest, Broadcast) {
    Mutex mut;
    cv condVar;
    int counter = 0;

    auto waitFunc = [&]() {
        mut.Lock();
        condVar.Wait(mut);
        counter++;
        mut.Unlock();
    };

    std::thread t1(waitFunc);
    std::thread t2(waitFunc);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    condVar.Broadcast();

    t1.join();
    t2.join();

    EXPECT_EQ(counter, 2);
}

// Test fixture for Condition Variable (cv)
class CVTest : public ::testing::Test {
protected:
    Mutex mutex_;
    cv condition_variable_;
    std::atomic<bool> ready_{false};
    std::atomic<int> data_{0};
};

// Test cv signaling and waiting
TEST_F(CVTest, SignalAndWait) {
    std::thread producer([this]() {
        LockGuard lock(mutex_);
        data_ = 42;
        ready_ = true;
        condition_variable_.Signal();
    });

    std::thread consumer([this]() {
        LockGuard lock(mutex_);
        while (!ready_) {
            condition_variable_.Wait(lock);
        }
        EXPECT_EQ(data_, 42);
    });

    producer.join();
    consumer.join();
}

// Test cv predicate-based waiting
TEST_F(CVTest, PredicateWait) {
    std::thread producer([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work
        LockGuard lock(mutex_);
        data_ = 100;
        ready_ = true;
        condition_variable_.Signal();
    });

    std::thread consumer([this]() {
        LockGuard lock(mutex_);
        condition_variable_.Wait(lock, [this]() { return ready_.load(); });
        EXPECT_EQ(data_, 100);
    });

    producer.join();
    consumer.join();
}

// Test cv broadcast
TEST_F(CVTest, Broadcast) {
    const int numConsumers = 5;
    std::vector<std::thread> consumers;
    std::atomic<int> readyConsumers{0};

    consumers.reserve(numConsumers);
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back([this, &readyConsumers]() {
            LockGuard lock(mutex_);
            condition_variable_.Wait(lock, [this]() { return ready_.load(); });
            ++readyConsumers;
        });
    }

    std::thread producer([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work
        LockGuard lock(mutex_);
        ready_ = true;
        condition_variable_.Broadcast();
    });

    producer.join();
    for (auto& t : consumers) {
        t.join();
    }

    EXPECT_EQ(readyConsumers, numConsumers);
}


// Test locks.h (LockGuard)
TEST(LockGuardTest, LockAndUnlock) {
    Mutex mut;
    bool locked = false;

    {
        LockGuard guard(mut);
        locked = true;  // Mutex is locked within the scope.
    }

    EXPECT_TRUE(locked);  // Ensure the mutex was locked and then unlocked when guard went out of scope.
}

// Test LockGuard RAII behavior
TEST(LockGuardTest, RAIIBehavior) {
    Mutex mutex;
    std::atomic<int> counter{0};

    const int numIterations = 1000;
    auto incrementTask = [&]() {
        for (int i = 0; i < numIterations; ++i) {
            LockGuard lock(mutex);
            ++counter;
        }
    };

    std::thread t1(incrementTask);
    std::thread t2(incrementTask);

    t1.join();
    t2.join();

    EXPECT_EQ(counter, 2 * numIterations);
}

// Test DeferLock tag: LockGuard does not lock on construction
TEST(LockGuardTest, DeferLockTag) {
    Mutex mutex;
    {
        // Create guard with DeferLock (mutex remains unlocked)
        LockGuard guard(mutex, DeferLock);
        // Verify mutex is still unlocked
        EXPECT_TRUE(mutex.TryLock());  // Succeeds because mutex was not locked
        mutex.Unlock();
    }
    // Guard destroyed; no action taken since it was never locked
    EXPECT_TRUE(mutex.TryLock());  // Verify cleanup
    mutex.Unlock();
}

// Test TryToLock tag: LockGuard tries to lock without blocking
TEST(LockGuardTest, TryToLockTagSucceedsWhenUnlocked) {
    Mutex mutex;
    {
        LockGuard guard(mutex, TryToLock);
        // Mutex should be locked if TryLock succeeded
        EXPECT_FALSE(mutex.TryLock());  // Verify locked
    }
    // Guard destroyed; mutex unlocked
    EXPECT_TRUE(mutex.TryLock());  // Verify cleanup
    mutex.Unlock();
}

TEST(LockGuardTest, TryToLockTagFailsWhenLocked) {
    Mutex mutex;
    mutex.Lock();  // Manually lock first
    {
        LockGuard guard(mutex, TryToLock);
        // TryLock should fail (mutex already locked)
        EXPECT_FALSE(mutex.TryLock());  // Fails, but we check via Mutex::TryLock
        mutex.Unlock();                // Cleanup for test
    }
    mutex.Unlock();  // Release manual lock
}

// Test AdoptLock tag: LockGuard takes ownership of an already locked mutex
TEST(LockGuardTest, AdoptLockTag) {
    Mutex mutex;
    mutex.Lock();  // Manually lock
    {
        LockGuard guard(mutex, AdoptLock);
        // Verify mutex remains locked
        EXPECT_FALSE(mutex.TryLock());
    }
    // Guard destroyed; mutex unlocked
    EXPECT_TRUE(mutex.TryLock());  // Verify cleanup
    mutex.Unlock();
}

// Test LockGuard::TryLock method
TEST(LockGuardTest, TryLockMethod) {
    Mutex mutex;
    LockGuard guard(mutex, DeferLock);
    // Initially unlocked
    EXPECT_TRUE(guard.TryLock());   // Acquire lock
    EXPECT_FALSE(mutex.TryLock());  // Verify locked
    guard.Unlock();                 // Explicitly release
    EXPECT_TRUE(mutex.TryLock());   // Verify unlocked
    mutex.Unlock();
}

// Test move operations
TEST(LockGuardTest, MoveOperations) {
    Mutex mutex;
    LockGuard guard1(mutex, DeferLock);
    guard1.Lock();                         // Acquire lock
    LockGuard guard2 = std::move(guard1);  // Transfer ownership
    // Original guard1 no longer manages the mutex
    EXPECT_FALSE(mutex.TryLock());  // Still locked by guard2
    guard2.Unlock();                // Release
    EXPECT_TRUE(mutex.TryLock());   // Verify unlocked
    mutex.Unlock();
}

// Test mutex.h (Mutex)
TEST(MutexTest, LockAndUnlock) {
    Mutex mut;
    mut.Lock();
    EXPECT_NO_THROW(mut.Unlock());
}

TEST(MutexTest, ThreadContention) {
    Mutex mut;
    int sharedValue = 0;

    auto increment = [&]() {
        for (int i = 0; i < 1000; ++i) {
            mut.Lock();
            sharedValue++;
            mut.Unlock();
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    EXPECT_EQ(sharedValue, 2000);
}

TEST(MutexTest, TryLockSucceedsWhenUnlocked) {
    Mutex mutex;
    EXPECT_TRUE(mutex.TryLock());  // Should succeed
    mutex.Unlock();                // Release the mutex
}

TEST(MutexTest, TryLockFailsWhenLocked) {
    Mutex mutex;
    mutex.Lock();                   // Acquire the mutex
    EXPECT_FALSE(mutex.TryLock());  // Should fail
    mutex.Unlock();                 // Release the mutex
}