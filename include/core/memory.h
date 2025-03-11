// various smart pointer implementations
// authors: @Anubhav652

#ifndef LIB_MEMORY_H
#define LIB_MEMORY_H

// To do: Custom deleter?

#include <cstddef>
#include <utility>

namespace core {
template<typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr){};

    ~UniquePtr() { delete ptr_; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    T* Get() const noexcept { return ptr_; }

    T* Release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;

        return tmp;
    }

    void Reset(T* rptr) noexcept {
        delete ptr_;

        ptr_ = rptr;
    }

    void Swap(UniquePtr<T>& other) noexcept {
        T* temp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp;
    }

    T& operator*() const noexcept { return *ptr_; }

    T* operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_ != nullptr; }

private:
    T* ptr_;
};

template<typename T>
bool operator==(const UniquePtr<T>& p, nullptr_t) noexcept {
    return p.Get() == nullptr;
}

template<typename T>
bool operator==(nullptr_t, const UniquePtr<T>& p) noexcept {
    return p.Get() == nullptr;
}

template<typename T>
bool operator!=(const UniquePtr<T>& p, nullptr_t) noexcept {
    return p.Get() != nullptr;
}

template<typename T>
bool operator!=(nullptr_t, const UniquePtr<T>& p) noexcept {
    return p.Get() != nullptr;
}

template<typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
    return UniquePtr<T>{new T(std::forward<Args>(args)...)};
}

}  // namespace core
#endif
