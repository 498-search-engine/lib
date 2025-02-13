#ifndef MEMORY_H
#define MEMORY_H

// To do: Custom deleter?

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

        return ptr_;
    }

    T* Reset(T* rptr) noexcept {
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

private:
    T* ptr_;
};

}  // namespace core
#endif