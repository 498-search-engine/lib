#ifndef LIB_SPAN_H
#define LIB_SPAN_H

#include <cstddef>

namespace core {

template<typename T>
class span {
public:
    using iterator = T*;
    using const_iterator = const T*;

    span(T* ptr, size_t size) : ptr_(ptr), size_(size) {}

    T* data() const { return ptr_; }
    size_t size() const { return size_; }

    iterator begin() const { return ptr_; }
    iterator end() const { return ptr_ + size_; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    T& operator[](size_t n) const { return *(ptr_ + n); }

    [[nodiscard]] span subspan(size_t offset, size_t size) const { return span{ptr_ + offset, size}; }

private:
    T* ptr_;
    size_t size_;
};

}  // namespace core

#endif
