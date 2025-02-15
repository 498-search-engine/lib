// wrapper for RAII style locking for pthread mutexes
// authors: @Anubhav652

#ifndef LIB_THREAD_LOCKS_H
#define LIB_THREAD_LOCKS_H

#include <core/mutex.h>

namespace core {

class LockGuard {
public:
    LockGuard(Mutex& m) : mut_(&m), locked_(true) { mut_->Lock(); }

    ~LockGuard() {
        if ((mut_ != nullptr) && locked_) {
            mut_->Unlock();
        }
    }

    void Lock() {
        mut_->Lock();
        locked_ = true;
    }

    void Unlock() {
        mut_->Unlock();
        locked_ = false;
    }

    LockGuard(LockGuard&& other) noexcept : mut_(other.mut_), locked_(other.locked_) { other.mut_ = nullptr; }

    LockGuard& operator=(LockGuard&& other) noexcept {
        mut_ = other.mut_;
        locked_ = other.locked_;

        other.mut_ = nullptr;

        return *this;
    }

private:
    Mutex* mut_;
    bool locked_;

    friend class cv;
};

}  // namespace core
#endif