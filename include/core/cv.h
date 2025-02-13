#ifndef CV_H
#define CV_H 

#include <core/mutex.h>

namespace core {

class cv {
public:
    cv() {
        pthread_cond_init(&_cv, NULL);
    };

    ~cv();

    void wait(mutex& mut) {
        pthread_cond_wait(&_cv, &mut._mutex);
    }

    void signal() {
        pthread_cond_signal(&_cv);
    }

    void broadcast() {
        pthread_cond_broadcast(&_cv);
    }

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
    pthread_cond_t _cv;
};

} // namespace core
#endif