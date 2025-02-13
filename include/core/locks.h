#ifndef THREAD_LOCKS_H
#define THREAD_LOCKS_H

#include <core/mutex.h>

namespace core {

class lock_guard {
    lock_guard(mutex &m) : mut(m) {
        mut.lock();
    }

    ~lock_guard() {
        mut.unlock();
    }

private:
    mutex &mut;
};

} // namespace core
#endif