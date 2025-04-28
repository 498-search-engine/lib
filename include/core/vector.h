/**
 * @file vector.h
 * @author Arnav Shah, Christopher Davis
 * @brief C++20 compliant std::vector implementation
 * @version 1.1
 * @date 2025-04-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <algorithm>
#include <compare>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace core {

template<typename T>
class Vector {
public:
    /* Types */
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* Constructors */
    constexpr Vector() noexcept :
        start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}

    explicit Vector(size_type count) : Vector() {
        allocateAndFill(count, T());
    }

    constexpr Vector(size_type count, const T& value) : Vector() {
        allocateAndFill(count, value);
    }

    template<class InputIt>
        requires(!std::is_integral_v<InputIt>)
    constexpr Vector(InputIt first, InputIt last) : Vector() {
        for (auto it = first; it != last; ++it)
            emplace_back(*it);
    }

    Vector(std::initializer_list<T> init) : Vector(init.begin(), init.end()) {}

    constexpr Vector(const Vector& other) : Vector() {
        reserve(other.size());
        for (const auto& elem: other)
            emplace_back(elem);
    }

    constexpr Vector(Vector&& other) noexcept
        : start_(other.start_), finish_(other.finish_), end_of_storage_(other.end_of_storage_)
    {
        other.start_ = other.finish_ = other.end_of_storage_ = nullptr; // make valid
    }

    constexpr Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            reserve(other.size());
            for (const auto& elem : other) {
                emplace_back(elem);
            }
        }
        return *this;
    }

    constexpr Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            deallocate();
            start_ = other.start_;
            finish_ = other.finish_;
            end_of_storage_ = other.end_of_storage_;
            other.start_ = other.finish_ = other.end_of_storage_ = nullptr;
        }
        return *this;
    }

    constexpr Vector& operator=(std::initializer_list<T> il) {
        assign(il);
        return *this;
    }

    ~Vector() {
        clear();
        deallocate();
    }

    /* Assign */
    constexpr void assign(size_type count, const T& value) {
        clear();
        reserve(count);
        for (size_type i = 0; i < count; ++i) {
            emplace_back(value);
        }
    }

    template<class InputIt>
        requires(!std::is_integral_v<InputIt>)
    constexpr void assign(InputIt first, InputIt last) {
        clear();
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    constexpr void assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    /* Element Access */
    constexpr reference at(size_type pos) {
        if (pos >= size()) throw std::out_of_range("vector::at");
        return start_[pos];
    }
    constexpr const_reference at(size_type pos) const {
        if (pos >= size()) throw std::out_of_range("vector::at");
        return start_[pos];
    }

    constexpr reference operator[](size_type pos) noexcept { return start_[pos]; }
    constexpr const_reference operator[](size_type pos) const noexcept { return start_[pos]; }

    constexpr reference front() { return *start_; }
    constexpr const_reference front() const { return *start_; }

    constexpr reference back() { return *(finish_ - 1); }
    constexpr const_reference back() const { return *(finish_ - 1); }

    constexpr pointer data() noexcept { return start_; }
    constexpr const_pointer data() const noexcept { return start_; }

    /* Iterators */
    constexpr iterator begin() noexcept { return start_; }
    constexpr const_iterator begin() const noexcept { return start_; }
    constexpr const_iterator cbegin() const noexcept { return start_; }
    constexpr iterator end() noexcept { return finish_; }
    constexpr const_iterator end() const noexcept { return finish_; }
    constexpr const_iterator cend() const noexcept { return finish_; }
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    /* Capacity */
    [[nodiscard]] constexpr bool empty() const noexcept { return start_ == finish_; }
    constexpr size_type size() const noexcept { return static_cast<size_type>(finish_ - start_); }
    constexpr size_type capacity() const noexcept { return static_cast<size_type>(end_of_storage_ - start_); }

    constexpr void reserve(size_type new_cap) {
        if (new_cap > capacity()) reallocate(new_cap);
    }

    constexpr void shrink_to_fit() {
        if (finish_ < end_of_storage_) reallocate(size());
    }

    /* Modifiers */
    constexpr void clear() noexcept {
        while (finish_ != start_) {
            (--finish_)->~T();
        }
    }

    constexpr iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    constexpr iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    constexpr iterator insert(const_iterator pos, size_type count, const T& value) {
        size_type idx = pos - start_;
        if (count == 0) return begin() + idx;
        grow_if_needed(count);
        pos = start_ + idx;
        shift_right(idx, count);
        for (size_type i = 0; i < count; ++i) {
            new (start_ + idx + i) T(value);
        }
        finish_ += count;
        return start_ + idx;
    }

    template<class InputIt>
        requires(!std::is_integral_v<InputIt>)
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type idx = pos - start_;
        size_type count = std::distance(first, last);
        if (count == 0) return begin() + idx;
        grow_if_needed(count);
        pos = start_ + idx;
        shift_right(idx, count);
        for (auto it = first; it != last; ++it) {
            new (start_ + idx++) T(*it);
        }
        finish_ += count;
        return start_ + (idx - count);
    }

    constexpr iterator insert(const_iterator pos, std::initializer_list<T> il) {
        return insert(pos, il.begin(), il.end());
    }

    constexpr iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }
    
    constexpr iterator erase(const_iterator first, const_iterator last) {
        iterator f = start_ + (first - start_);
        iterator l = start_ + (last - start_);
        iterator it = f;
        for (; l != finish_; ++f, ++l) {
            *f = std::move(*l);
        }
        while (finish_ != f) (--finish_)->~T();
        return it;
    }

    template<typename... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args) {
        size_type idx = pos - start_;
        if (finish_ == end_of_storage_) grow_if_needed(1);
        pos = start_ + idx;
        shift_right(idx, 1);
        new (const_cast<pointer>(pos)) T(std::forward<Args>(args)...);
        ++finish_;
        return start_ + idx;
    }

    template<typename... Args>
    constexpr reference emplace_back(Args&&... args) {
        if (finish_ == end_of_storage_) grow_if_needed(1);
        new (finish_) T(std::forward<Args>(args)...);
        return *finish_++;
    }

    constexpr void push_back(const T& value) { emplace_back(value); }
    constexpr void push_back(T&& value) { emplace_back(std::forward<T>(value)); }

    constexpr void pop_back() { (--finish_)->~T(); }

    constexpr void resize(size_type count) { resize(count, T()); }
    constexpr void resize(size_type count, const T& value) {
        if (count < size()) {
            while (size() > count) pop_back();
        } else if (count > size()) {
            reserve(count);
            while (size() < count) emplace_back(value);
        }
    }

    // TODO: make conditional noexcept with allocator_traits
    constexpr void swap(Vector& other) noexcept {
        std::swap(start_, other.start_);
        std::swap(finish_, other.finish_);
        std::swap(end_of_storage_, other.end_of_storage_);
    }

    /* Comparison */
    friend constexpr bool operator==(const Vector& a, const Vector& b) {
        if (a.size() != b.size()) return false;
        for (size_type i = 0; i < a.size(); ++i) {
            if (!(a.start_[i] == b.start_[i])) return false;
        }
        return true;
    }

    friend constexpr auto operator<=>(const Vector& a, const Vector& b) {
        return std::lexicographical_compare_three_way(
            a.begin(), a.end(), b.begin(), b.end());
    }

private: /* Private member fields */
    T* start_;
    T* finish_; // start + size
    T* end_of_storage_;

private: 
    /* Helpers */
    constexpr void allocateAndFill(size_type n, const T& value) {
        start_ = static_cast<T*>(::operator new(n * sizeof(T)));
        finish_ = start_;
        end_of_storage_ = start_ + n;
        for (size_type i = 0; i < n; ++i) {
            new (finish_++) T(value);
        }
    }

    constexpr void deallocate() noexcept {
        ::operator delete(start_);
    }

    constexpr void reallocate(size_type new_cap) {
        T* new_start = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        T* new_finish = new_start;
        for (T* p = start_; p != finish_; ++p) {
            new (new_finish++) T(std::move(*p));
            p->~T();
        }
        deallocate();
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_cap;
    }

    constexpr void grow_if_needed(size_type n) {
        size_type old_cap = capacity();
        size_type new_cap = old_cap ? old_cap * 2 : n;
        if (new_cap < size() + n) new_cap = size() + n;
        reallocate(new_cap);
    }

    constexpr void shift_right(size_type idx, size_type count) {
        pointer src = start_ + size() - 1;
        pointer dst = src + count;
        for (; src >= start_ + idx; --src, --dst) {
            new (dst) T(std::move(*src));
            src->~T();
        }
    }

};
    
}  // namespace core

#endif
