#ifndef THREAD_LOCKS_H
#define THREAD_LOCKS_H

#include <core/mutex.h>

namespace core {

class LockGuard {
    LockGuard(Mutex& m) : mut_(m) { mut_.Lock(); }

    ~LockGuard() { mut_.Unlock(); }

private:
    Mutex& mut_;
};

}  // namespace core
#endif