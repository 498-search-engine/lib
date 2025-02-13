// wrapper for pthreads in C++ style
// authors: @Anubhav652

#ifndef THREAD_H
#define THREAD_H

#include <core/memory.h>
#include <pthread.h>
#include <stdexcept>
#include <utility>


template<typename T>
static void* ThreadEntry(void* arg) {
    core::UniquePtr<T> task(static_cast<T*>(arg));
    (*task)();
    return nullptr;
}

namespace core {

template<typename Function, typename... Args>
class Thread {
public:

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Thread(Function&& f, Args&&... args) : end_decided_(false) {
        /*
         * static_cast<Args&&> is equivalent to std::forward here
         */
        auto lambda = [func = std::forward<Function>(f), ...capturedArgs = std::forward<Args>(args)]() mutable {
            func(std::forward<Args>(capturedArgs)...);
        };

        using Lambda_t = decltype(lambda);
        Lambda_t* task = new Lambda_t(std::move(lambda));
        int result = pthread_create(&thread_id_, nullptr, &ThreadEntry<Lambda_t>, task);
        if (result != 0) {
            throw std::runtime_error("failed to create pthread");
        }
    }

    ~Thread() {
        if (!end_decided_) {
            pthread_detach(thread_id_);
        }
    }

    void Join() {
        pthread_join(thread_id_, nullptr);
        end_decided_ = true;
    }

    void Detach() {
        pthread_detach(thread_id_);
        end_decided_ = true;
    }

    /*
     * Disable the copy constructor and copy assignment operator.
     */
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    /*
     * Move constructor and move assignment operator.
     */
    Thread(Thread&& other) noexcept {
        thread_id_ = other.thread_id_;
        end_decided_ = other.end_decided_;

        other.thread_id_ = 0;
        other.end_decided_ = true;
    }

    Thread& operator=(Thread&& other) noexcept {
        thread_id_ = other.thread_id_;
        end_decided_ = other.end_decided_;

        other.thread_id_ = 0;
        other.end_decided_ = true;
    }

private:
    pthread_t thread_id_;
    bool end_decided_;
};

}  // namespace core
#endif