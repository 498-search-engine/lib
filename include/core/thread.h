#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

template <typename Function, typename ... Args>
class thread {
public:
    thread(Function &&func, Args&&... args) : end_decided(false) {
        /*
        * static_cast<Args&&> is equivalent to std::forward here
        */
        auto wrapper = [&func, ...args = static_cast<Args&&>(args)](void *arg) -> void * {
            func(args...);
            return nullptr;
        };

        pthread_create(&thread_id, nullptr, wrapper, nullptr);
    }

    ~thread() {
        if (!end_decided) {
            pthread_detach(thread_id);
        }
    }

    void join() {
        pthread_join(thread_id, nullptr);
        end_decided = true;
    }                      

    void detach() {
        pthread_detach(thread_id);
        end_decided = true;
    }

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    thread(thread&& other) {
        thread_id = other.thread_id;
        end_decided = other.end_decided;

        other.thread_id = 0;
        other.end_decided = true;
    }

    thread& operator=(thread&& other) {
        thread_id = other.thread_id;
        end_decided = other.end_decided;

        other.thread_id = 0;
        other.end_decided = true;
    }

private:
    pthread_t thread_id;
    bool end_decided;

};

#endif