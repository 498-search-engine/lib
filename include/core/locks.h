#ifndef THREAD_LOCKS_H
#define THREAD_LOCKS_H

#include <core/mutex.h>

namespace core {

class LockGuard {
    LockGuard(Mutex& m) : mut_(m), locked_(true) { mut_.Lock(); }

    ~LockGuard() {
        if (locked_) {
            mut_.Unlock();
        }
    }

    void Lock() {
        mut_.Lock();
        locked_ = true;
    }
    void Unlock() {
        mut_.Unlock();
        locked_ = false;
    }

private:
    Mutex& mut_;
    bool locked_;
};

}  // namespace core
#endif