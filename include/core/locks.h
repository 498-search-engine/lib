// wrapper for RAII style locking for pthread mutexes
// authors: @Anubhav652

#ifndef LIB_THREAD_LOCKS_H
#define LIB_THREAD_LOCKS_H

#include <core/mutex.h>

namespace core {

/*
    Tags for lock guard constructor
*/
struct DeferLockT {
    explicit DeferLockT() = default;
};
struct TryToLockT {
    explicit TryToLockT() = default;
};
struct AdoptLockT {
    explicit AdoptLockT() = default;
};

constexpr DeferLockT DeferLock{};
constexpr TryToLockT TryToLock{};
constexpr AdoptLockT AdoptLock{};

class LockGuard {
public:
    LockGuard(Mutex& m) : mut_(&m), locked_(true) { mut_->Lock(); }
    // NOLINTNEXTLINE(misc-unused-parameters)
    LockGuard(Mutex& m, core::DeferLockT _) : mut_(&m) { locked_ = false; }
    // NOLINTNEXTLINE(misc-unused-parameters)
    LockGuard(Mutex& m, core::TryToLockT _) : mut_(&m) { locked_ = mut_->TryLock(); }
    // NOLINTNEXTLINE(misc-unused-parameters)
    LockGuard(Mutex& m, core::AdoptLockT _) : mut_(&m) { locked_ = true; }

    ~LockGuard() {
        if ((mut_ != nullptr) && locked_) {
            mut_->Unlock();
        }
    }

    void Lock() {
        mut_->Lock();
        locked_ = true;
    }

    bool TryLock() {
        locked_ = mut_->TryLock();
        return locked_;
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