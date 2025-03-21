#ifndef LIB_ITERATOR_H
#define LIB_ITERATOR_H

#include <cstddef>
#include <type_traits>

namespace core {

template<typename T>
class Iterator {};

template<typename T, typename PointerType = T*>
class PointerIteratorWrapper {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = PointerType;
    using reference = typename std::conditional_t<std::is_const_v<T>, const typename std::remove_const_t<T>&, T&>;

    constexpr PointerIteratorWrapper() : ptr_(nullptr) {}
    constexpr explicit PointerIteratorWrapper(pointer ptr) : ptr_(ptr) {}

    // Allow conversion from non-const to const iterator
    template<typename U>
        requires std::is_const_v<T> && std::is_same_v<U, std::remove_const_t<T>>
    constexpr PointerIteratorWrapper(const PointerIteratorWrapper<U>& other) : ptr_(other.ptr_) {}

    constexpr reference operator*() const { return *ptr_; }
    constexpr pointer operator->() const { return ptr_; }
    constexpr reference operator[](difference_type n) const { return ptr_[n]; }

    constexpr PointerIteratorWrapper& operator++() {
        ++ptr_;
        return *this;
    }

    constexpr PointerIteratorWrapper operator++(int) {
        PointerIteratorWrapper tmp(*this);
        ++ptr_;
        return tmp;
    }

    constexpr PointerIteratorWrapper& operator--() {
        --ptr_;
        return *this;
    }

    constexpr PointerIteratorWrapper operator--(int) {
        PointerIteratorWrapper tmp(*this);
        --ptr_;
        return tmp;
    }

    constexpr PointerIteratorWrapper& operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    constexpr PointerIteratorWrapper& operator-=(difference_type n) {
        ptr_ -= n;
        return *this;
    }

    constexpr PointerIteratorWrapper operator+(difference_type n) const { return PointerIteratorWrapper(ptr_ + n); }
    constexpr PointerIteratorWrapper operator-(difference_type n) const { return PointerIteratorWrapper(ptr_ - n); }
    constexpr difference_type operator-(const PointerIteratorWrapper& other) const { return ptr_ - other.ptr_; }

    constexpr bool operator==(const PointerIteratorWrapper& other) const { return ptr_ == other.ptr_; }
    constexpr bool operator!=(const PointerIteratorWrapper& other) const { return ptr_ != other.ptr_; }
    constexpr bool operator<(const PointerIteratorWrapper& other) const { return ptr_ < other.ptr_; }
    constexpr bool operator>(const PointerIteratorWrapper& other) const { return ptr_ > other.ptr_; }
    constexpr bool operator<=(const PointerIteratorWrapper& other) const { return ptr_ <= other.ptr_; }
    constexpr bool operator>=(const PointerIteratorWrapper& other) const { return ptr_ >= other.ptr_; }

    // Friend declaration to allow access to ptr_ in the conversion constructor
    template<typename U, typename P>
    friend class PointerIteratorWrapper;

private:
    pointer ptr_;
};

template<typename T, typename PointerType>
constexpr PointerIteratorWrapper<T, PointerType>
operator+(typename PointerIteratorWrapper<T, PointerType>::difference_type n,
          const PointerIteratorWrapper<T, PointerType>& iter) {
    return iter + n;
}

}  // namespace core

#endif
