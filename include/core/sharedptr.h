// wait-free shared ptr impl
// refcounting heavily inspired by: https://www.youtube.com/watch?v=kPh8pod0-gk
// authors: @Anubhav652

#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <atomic>
#include <cstdint>

namespace {
class Counter {
public:
    bool IncrementIfNotZero() { return (counter_.fetch_add(1) & is_zero_) == 0; }

    bool Decrement() {
        if (counter_.fetch_sub(1) == 1) {
            uint64_t e = 0;
            if (counter_.compare_exchange_strong(e, is_zero_) ||
                static_cast<bool>(e & helped_) && static_cast<bool>(counter_.exchange(is_zero_) & helped_)) {
                return true;
            };
        }

        return false;
    }

    uint64_t Read() {
        auto val = counter_.load();
        if (val == 0 && counter_.compare_exchange_strong(val, is_zero_ | helped_)) {
            return 0;
        }
        return static_cast<bool>(val & is_zero_) ? 0 : val;
    }

private:
    uint64_t is_zero_ = 1ULL << 63;
    uint64_t helped_ = 1ULL << 62;
    std::atomic<uint64_t> counter_{1};
};
};  // namespace

namespace core {

template<typename T>
class SharedPtr {
public:
    explicit SharedPtr(T* ptr = nullptr) : counter_(new Counter()), ptr_(ptr) {};

    SharedPtr(const SharedPtr& other) : counter_(other.counter_), ptr_(other.ptr_) { counter_->IncrementIfNotZero(); }

    ~SharedPtr() { Decrement(); }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            counter_ = other.counter_;
            ptr_ = other.ptr_;

            counter_->IncrementIfNotZero();
        }

        return *this;
    };

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
        other.ptr_ = nullptr;
        other.counter_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            Decrement();

            ptr_ = other.ptr_;
            counter_ = other.counter_;
        }

        return *this;
    }

    T* Get() const noexcept { return ptr_; }

    T& operator*() const noexcept { return *ptr_; }

    T* operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_ != nullptr; }

private:
    T* ptr_;
    Counter* counter_;

    void Decrement() {
        if (static_cast<bool>(counter_) && counter_->Decrement()) {
            delete ptr_;
            delete counter_;

            ptr_ = nullptr;
            counter_ = nullptr;
        }
    }
};

template<typename T>
bool operator==(const SharedPtr<T>& p, std::nullptr_t) noexcept {
    return p.Get() == nullptr;
}

template<typename T>
bool operator==(std::nullptr_t, const SharedPtr<T>& p) noexcept {
    return p.Get() == nullptr;
}

template<typename T>
bool operator!=(const SharedPtr<T>& p, std::nullptr_t) noexcept {
    return p.Get() != nullptr;
}

template<typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T>& p) noexcept {
    return p.Get() != nullptr;
}

template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>{new T(std::forward<Args>(args)...)};
}
}  // namespace core
#endif