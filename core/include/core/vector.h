#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <cstddef>

template<typename T>
class Vector {
public:
    // Member Types
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    // Constructors
    Vector() noexcept : data_(nullptr), size_(0), capacity_(0) {}
    explicit Vector(size_type count) : data_(new T[count]{}), size_(0), capacity_(0) {}
    Vector(size_type count, const T& value) : data_

private:
    pointer Allocate(size_type n)

private:
    T* data_;
    size_type size_;
    size_type capacity_;
};

#endif
