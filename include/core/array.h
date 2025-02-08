#ifndef LIB_ARRAY_H
#define LIB_ARRAY_H

#include <cstddef>
#include <utility>

namespace core {

template<typename T, size_t N>
struct Array {
    using iterator = T*;
    using const_iterator = const T*;

    T __elements_[N];

    T* Data() { return __elements_; }
    const T* Data() const { return __elements_; }

    constexpr size_t Size() const { return N; }
    constexpr bool Empty() const { return N == 0; }

    iterator begin() { return __elements_; }
    iterator end() { return __elements_ + N; }
    const_iterator begin() const { return __elements_; }
    const_iterator end() const { return __elements_ + N; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    T& operator[](size_t n) { return __elements_[n]; }
    const T& operator[](size_t n) const { return __elements_[n]; }

    void Fill(const T& val) {
        for (size_t i = 0; i < N; ++i) {
            __elements_[i] = val;
        }
    }

    void Swap(Array& other) { std::swap(__elements_, other.__elements_); }
};

}  // namespace core

#endif
