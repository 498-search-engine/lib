#ifndef CV_H
#define CV_H

#include <core/mutex.h>

namespace core {

class cv {
public:
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    cv() { pthread_cond_init(&cv_, NULL); };

    ~cv() = default;

    void Wait(Mutex& mut) { pthread_cond_wait(&cv_, &mut.mutex_); }

    void Signal() { pthread_cond_signal(&cv_); }

    void Broadcast() { pthread_cond_broadcast(&cv_); }

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    cv(const cv&) = delete;
    cv& operator=(const cv&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    cv(cv&&) = delete;
    cv& operator=(cv&&) = delete;

private:
    pthread_cond_t cv_;
};

}  // namespace core
#endif