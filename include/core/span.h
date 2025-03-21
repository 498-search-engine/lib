#ifndef LIB_SPAN_H
#define LIB_SPAN_H

#include "core/iterator.h"

#include <cstddef>

namespace core {

template<typename T>
class Span {
public:
    using iterator = PointerIteratorWrapper<T>;
    using const_iterator = PointerIteratorWrapper<const T>;

    constexpr Span() : ptr_(nullptr), size_(0) {}
    constexpr Span(T* ptr, size_t size) : ptr_(ptr), size_(size) {}

    template<size_t N>
    constexpr Span(T (&arr)[N]) : ptr_(arr), size_(N) {}

    constexpr T* Data() const { return ptr_; }
    constexpr size_t Size() const { return size_; }
    constexpr bool Empty() const { return size_ == 0; }

    constexpr T& Front() const { return ptr_[0]; }
    constexpr T& Back() const { return ptr_[size_ - 1]; }

    constexpr iterator begin() const { return iterator(ptr_); }
    constexpr iterator end() const { return iterator(ptr_ + size_); }
    constexpr const_iterator cbegin() const { return begin(); }
    constexpr const_iterator cend() const { return end(); }

    constexpr T& operator[](size_t n) const { return *(ptr_ + n); }

    [[nodiscard]] constexpr Span Subspan(size_t offset) const { return Span{ptr_ + offset, size_ - offset}; }
    [[nodiscard]] constexpr Span Subspan(size_t offset, size_t size) const { return Span{ptr_ + offset, size}; }

private:
    T* ptr_;
    size_t size_;
};

}  // namespace core

#endif
