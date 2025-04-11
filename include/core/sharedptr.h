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

    SharedPtr(const SharedPtr&) = delete;
    SharedPtr& operator=(const SharedPtr&) = delete;

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
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
};
}  // namespace core
#endif