#ifndef LIB_SPAN_H
#define LIB_SPAN_H

#include <cstddef>

namespace core {

template<typename T>
class Span {
public:
    Span(T* ptr, size_t size) : ptr_(ptr), size_(size) {}

    T* Data() const { return ptr_; }
    size_t Size() const { return size_; }

    T* begin() const { return ptr_; }
    T* end() const { return ptr_ + size_; }
    const T* cbegin() const { return begin(); }
    const T* cend() const { return end(); }

    T& operator[](size_t n) const { return *(ptr_ + n); }

    [[nodiscard]] Span Subspan(size_t offset, size_t size) { return Span{ptr_ + offset, size}; }

private:
    T* ptr_;
    size_t size_;
};

}  // namespace core

#endif
