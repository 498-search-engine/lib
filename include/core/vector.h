// vector.h
//
//  Starter file for a Vector template

#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <initializer_list>
#include <iostream>
#include <type_traits>
#include <utility>

namespace core {


template<typename T>
class Vector {
public:
    // Default Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Constructs an empty Vector with capacity 0
    Vector() : arr(nullptr), sz(0), cap(0) {}

    // Destructor
    // REQUIRES: Nothing
    // MODIFIES: Destroys *this
    // EFFECTS: Performs any neccessary clean up operations
    ~Vector() { delete[] arr; }

    // Resize Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Constructs a Vector with size num_elements,
    //    all default constructed
    Vector(size_t num_elements) : sz(num_elements), cap(num_elements) { arr = new T[cap]{}; }

    // Fill Constructor
    // REQUIRES: Capacity > 0
    // MODIFIES: *this
    // EFFECTS: Creates a Vector with size num_elements, all assigned to val
    Vector(size_t num_elements, const T& val) : sz(num_elements), cap(num_elements) {
        arr = new T[cap];
        for (int i = 0; i < cap; i++)
            arr[i] = val;
    }

    // Copy Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Creates a clone of the Vector other
    Vector(const Vector<T>& other) : sz(other.sz), cap(other.cap) {
        arr = new T[cap];
        for (size_t i = 0; i < sz; ++i)
            arr[i] = other.arr[i];
    }

    Vector(const std::initializer_list<T>& il) {
        for (auto i : il) {
            push_back(i);
        }
    }

    Vector& operator=(const std::initializer_list<T>& il) {
        for (auto i : il) {
            push_back(i);
        }
    }

    // Assignment operator
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Duplicates the state of other to *this
    Vector& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] arr;
            sz = other.sz;
            cap = other.cap;
            arr = new T[cap];
            for (size_t i = 0; i < sz; ++i)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    void clear() { sz = 0; }

    // Move Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this, leaves other in a default constructed state
    // EFFECTS: Takes the data from other into a newly constructed Vector
    Vector(Vector<T>&& other) noexcept : arr(other.arr), sz(other.sz), cap(other.cap) {
        other.arr = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Move Assignment Operator
    // REQUIRES: Nothing
    // MODIFIES: *this, leaves other in a default constructed state
    // EFFECTS: Takes the data from other in constant time
    Vector& operator=(Vector<T>&& other) {
        if (this != &other) {
            delete[] arr;
            arr = other.arr;
            sz = other.sz;
            cap = other.cap;
            other.arr = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // REQUIRES: new_capacity > capacity( )
    // MODIFIES: capacity( )
    // EFFECTS: Ensures that the Vector can contain size( ) = new_capacity
    //    elements before having to reallocate
    void reserve(size_t newCapacity) {
        if (newCapacity > cap) {
            T* tmp = new T[newCapacity];
            for (size_t i = 0; i < sz; i++)
                tmp[i] = std::move(arr[i]);
            delete[] arr;
            arr = tmp;
            cap = newCapacity;
        }
    }

    void resize(size_t newCapacity) {
        reserve(newCapacity);
        sz = newCapacity;
    }

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the number of elements in the Vector
    size_t size() const { return sz; }

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the maximum size the Vector can attain before resizing
    size_t capacity() const { return cap; }

    // REQUIRES: 0 <= i < size( )
    // MODIFIES: Allows modification of data[i]
    // EFFECTS: Returns a mutable reference to the i'th element
    T& operator[](size_t i) { return arr[i]; }

    // REQUIRES: 0 <= i < size( )
    // MODIFIES: Nothing
    // EFFECTS: Get a const reference to the ith element
    const T& operator[](size_t i) const { return arr[i]; }

    // REQUIRES: Nothing
    // MODIFIES: this, size( ), capacity( )
    // EFFECTS: Appends the element x to the Vector, allocating
    //    additional space if neccesary
    void push_back(const T& x) {
        if (sz == cap)
            reserve(cap > 0 ? cap * 2 : 1);
        arr[sz] = x;
        sz++;
    }

    // REQUIRES: Nothing
    // MODIFIES: this, size( )
    // EFFECTS: Removes the last element of the Vector,
    //    leaving capacity unchanged
    void pop_back() {
        if (sz > 0)
            sz--;
    }

    void insert(size_t pos, const T& x) {
        size_t p = static_cast<size_t>(pos);
        if (p > sz)
            throw std::out_of_range("Vector::insert: position out of range");
        if (sz == cap)
            reserve(cap > 0 ? cap * 2 : 1);
        for (size_t i = sz; i > p; --i)
            arr[i] = std::move(arr[i - 1]);
        arr[p] = x;
        ++sz;
    }

    struct iterator {
        T* ptr;

        iterator(T* p) : ptr(p) {}

        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }

        iterator& operator++() {
            ++ptr;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }

        iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }

        iterator operator-(ptrdiff_t n) const { return iterator(ptr - n); }

        ptrdiff_t operator-(const iterator& other) const { return ptr - other.ptr; }
    };

    iterator begin() { return iterator(arr); }
    iterator end() { return iterator(arr + sz); }
    const iterator begin() const { return iterator(arr); }
    const iterator end() const { return iterator(arr + sz); }

    void insert(iterator it, const T& x) {
        size_t pos = static_cast<size_t>(it - begin());
        insert(pos, x);
    }

   
    bool empty() const { return sz == 0; }

    T* data() { return arr; }

    const T* data() const { return arr; }

    void insert(iterator pos, iterator first, iterator last) {
        size_t p = static_cast<size_t>(pos - begin());
        size_t count = static_cast<size_t>(last - first);

        if (p > sz) {
            throw std::out_of_range("Vector::insert: position out of range");
        }

        if (sz + count > cap) {
            reserve(std::max(cap * 2, sz + count));
        }

        for (size_t i = sz + count - 1; i >= p + count; --i) {
            arr[i] = std::move(arr[i - count]);
            if (i == 0)
                break;  
        }

        for (size_t i = 0; i < count; ++i) {
            arr[p + i] = *(first + i);
        }

        sz += count;
    }


private:
    // TODO
    T* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
};
}  // namespace core

#endif
