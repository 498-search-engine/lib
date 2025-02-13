#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class cv;

namespace core {
class mutex {
public:
    mutex();
    ~mutex();

    void lock() {
        pthread_mutex_lock(&_mutex);
    };
    void unlock() {
        pthread_mutex_lock(&_mutex);
    };

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    mutex(mutex&& other) = delete;
    mutex& operator=(mutex&&) = delete;

private:
    pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

    friend class cv;
};

} // namespace core
#endif