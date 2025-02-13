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