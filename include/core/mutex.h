#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class cv;

namespace core {
class Mutex {
public:
    Mutex() = default;
    ~Mutex() = default;

    void Lock() { pthread_mutex_lock(&mutex_); };
    void Unlock() { pthread_mutex_lock(&mutex_); };

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    Mutex(Mutex&& other) = delete;
    Mutex& operator=(Mutex&&) = delete;

private:
    pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;

    friend class cv;
};

}  // namespace core
#endif