#ifndef MEMORY_H
#define MEMORY_H

// To do: Custom deleter?

namespace core {
template <typename T>
class unique_ptr {
public:
    explicit unique_ptr(T* ptr = nullptr) : ptr(ptr) {};

    ~unique_ptr() {
        delete ptr;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        return *this;
    }

    T* get() const noexcept {
        return ptr;
    }

    T* release() noexcept {
        T* tmp = ptr;
        ptr = nullptr;

        return ptr;
    }

    T* reset(T* rptr) noexcept {
        delete ptr;

        ptr = rptr; 
    }

    void swap(unique_ptr<T>& other) noexcept {
        T* temp = ptr;
        ptr = other.ptr;
        other.ptr = temp;
    }

private:
    T *ptr;

};

}  // namespace core
#endif