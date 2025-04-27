// // vector.h
// //
// //  Starter file for a Vector template

// #ifndef LIB_VECTOR_H
// #define LIB_VECTOR_H

// #include <initializer_list>
// #include <iostream>
// #include <type_traits>
// #include <utility>

// namespace core {


// template<typename T>
// class Vector {
// public:
//     // Default Constructor
//     // REQUIRES: Nothing
//     // MODIFIES: *this
//     // EFFECTS: Constructs an empty Vector with capacity 0
//     Vector() : arr(nullptr), sz(0), cap(0) {}

//     // Destructor
//     // REQUIRES: Nothing
//     // MODIFIES: Destroys *this
//     // EFFECTS: Performs any neccessary clean up operations
//     ~Vector() { delete[] arr; }

//     // Resize Constructor
//     // REQUIRES: Nothing
//     // MODIFIES: *this
//     // EFFECTS: Constructs a Vector with size num_elements,
//     //    all default constructed
//     Vector(size_t num_elements) : sz(num_elements), cap(num_elements) { arr = new T[cap]{}; }

//     // Fill Constructor
//     // REQUIRES: Capacity > 0
//     // MODIFIES: *this
//     // EFFECTS: Creates a Vector with size num_elements, all assigned to val
//     Vector(size_t num_elements, const T& val) : sz(num_elements), cap(num_elements) {
//         arr = new T[cap];
//         for (int i = 0; i < cap; i++)
//             arr[i] = val;
//     }

//     // Copy Constructor
//     // REQUIRES: Nothing
//     // MODIFIES: *this
//     // EFFECTS: Creates a clone of the Vector other
//     Vector(const Vector<T>& other) : sz(other.sz), cap(other.cap) {
//         arr = new T[cap];
//         for (size_t i = 0; i < sz; ++i)
//             arr[i] = other.arr[i];
//     }

//     Vector(const std::initializer_list<T>& il) {
//         for (auto i : il) {
//             push_back(i);
//         }
//     }

//     Vector& operator=(const std::initializer_list<T>& il) {
//         for (auto i : il) {
//             push_back(i);
//         }
//     }

//     // Assignment operator
//     // REQUIRES: Nothing
//     // MODIFIES: *this
//     // EFFECTS: Duplicates the state of other to *this
//     Vector& operator=(const Vector<T>& other) {
//         if (this != &other) {
//             delete[] arr;
//             sz = other.sz;
//             cap = other.cap;
//             arr = new T[cap];
//             for (size_t i = 0; i < sz; ++i)
//                 arr[i] = other.arr[i];
//         }
//         return *this;
//     }

//     void clear() { sz = 0; }

//     // Move Constructor
//     // REQUIRES: Nothing
//     // MODIFIES: *this, leaves other in a default constructed state
//     // EFFECTS: Takes the data from other into a newly constructed Vector
//     Vector(Vector<T>&& other) noexcept : arr(other.arr), sz(other.sz), cap(other.cap) {
//         other.arr = nullptr;
//         other.sz = 0;
//         other.cap = 0;
//     }

//     // Move Assignment Operator
//     // REQUIRES: Nothing
//     // MODIFIES: *this, leaves other in a default constructed state
//     // EFFECTS: Takes the data from other in constant time
//     Vector& operator=(Vector<T>&& other) {
//         if (this != &other) {
//             delete[] arr;
//             arr = other.arr;
//             sz = other.sz;
//             cap = other.cap;
//             other.arr = nullptr;
//             other.sz = 0;
//             other.cap = 0;
//         }
//         return *this;
//     }

//     // REQUIRES: new_capacity > capacity( )
//     // MODIFIES: capacity( )
//     // EFFECTS: Ensures that the Vector can contain size( ) = new_capacity
//     //    elements before having to reallocate
//     void reserve(size_t newCapacity) {
//         if (newCapacity > cap) {
//             T* tmp = new T[newCapacity];
//             for (size_t i = 0; i < sz; i++)
//                 tmp[i] = std::move(arr[i]);
//             delete[] arr;
//             arr = tmp;
//             cap = newCapacity;
//         }
//     }

//     void resize(size_t newCapacity) {
//         reserve(newCapacity);
//         sz = newCapacity;
//     }

//     // REQUIRES: Nothing
//     // MODIFIES: Nothing
//     // EFFECTS: Returns the number of elements in the Vector
//     size_t size() const { return sz; }

//     // REQUIRES: Nothing
//     // MODIFIES: Nothing
//     // EFFECTS: Returns the maximum size the Vector can attain before resizing
//     size_t capacity() const { return cap; }

//     // REQUIRES: 0 <= i < size( )
//     // MODIFIES: Allows modification of data[i]
//     // EFFECTS: Returns a mutable reference to the i'th element
//     T& operator[](size_t i) { return arr[i]; }

//     // REQUIRES: 0 <= i < size( )
//     // MODIFIES: Nothing
//     // EFFECTS: Get a const reference to the ith element
//     const T& operator[](size_t i) const { return arr[i]; }

//     // REQUIRES: Nothing
//     // MODIFIES: this, size( ), capacity( )
//     // EFFECTS: Appends the element x to the Vector, allocating
//     //    additional space if neccesary
//     void push_back(const T& x) {
//         if (sz == cap)
//             reserve(cap > 0 ? cap * 2 : 1);
//         arr[sz] = x;
//         sz++;
//     }

//     // REQUIRES: Nothing
//     // MODIFIES: this, size( )
//     // EFFECTS: Removes the last element of the Vector,
//     //    leaving capacity unchanged
//     void pop_back() {
//         if (sz > 0)
//             sz--;
//     }

//     void insert(size_t pos, const T& x) {
//         size_t p = static_cast<size_t>(pos);
//         if (p > sz)
//             throw std::out_of_range("Vector::insert: position out of range");
//         if (sz == cap)
//             reserve(cap > 0 ? cap * 2 : 1);
//         for (size_t i = sz; i > p; --i)
//             arr[i] = std::move(arr[i - 1]);
//         arr[p] = x;
//         ++sz;
//     }

//     struct iterator {
//         T* ptr;

//         iterator(T* p) : ptr(p) {}

//         T& operator*() const { return *ptr; }
//         T* operator->() const { return ptr; }

//         iterator& operator++() {
//             ++ptr;
//             return *this;
//         }
//         iterator operator++(int) {
//             iterator tmp = *this;
//             ++ptr;
//             return tmp;
//         }

//         bool operator==(const iterator& other) const { return ptr == other.ptr; }
//         bool operator!=(const iterator& other) const { return ptr != other.ptr; }

//         iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }

//         iterator operator-(ptrdiff_t n) const { return iterator(ptr - n); }

//         ptrdiff_t operator-(const iterator& other) const { return ptr - other.ptr; }
//     };

//     iterator begin() { return iterator(arr); }
//     iterator end() { return iterator(arr + sz); }
//     const iterator begin() const { return iterator(arr); }
//     const iterator end() const { return iterator(arr + sz); }

//     void insert(iterator it, const T& x) {
//         size_t pos = static_cast<size_t>(it - begin());
//         insert(pos, x);
//     }

   
//     bool empty() const { return sz == 0; }

//     T* data() { return arr; }

//     const T* data() const { return arr; }

//     template<typename InputIt>
//     void insert(iterator pos, InputIt first, InputIt last) {
//         size_t insert_pos = static_cast<size_t>(pos - begin());
//         size_t num_elements = static_cast<size_t>(last - first);
    
//         if (sz + num_elements > cap) {
//             reserve(std::max(cap * 2, sz + num_elements));
//         }
    
//         // Shift existing elements to the right
//         for (size_t i = sz; i > insert_pos; --i) {
//             arr[i + num_elements - 1] = std::move(arr[i - 1]);
//         }
    
//         // Insert new elements
//         for (size_t i = 0; i < num_elements; ++i) {
//             arr[insert_pos + i] = *(first + i);  // 🛠️ FIXED: dereference after moving first + i
//         }
    
//         sz += num_elements;
//     }
    

// private:
//     // TODO
//     T* arr = nullptr;
//     size_t sz = 0;
//     size_t cap = 0;
// };
// }  // namespace core

// #endif


#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <initializer_list>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <algorithm>

namespace core {

template<typename T>
class Vector {
public:
    constexpr Vector() noexcept : arr(nullptr), sz(0), cap(0) {}

    constexpr ~Vector() {
        delete[] arr;
    }

    constexpr Vector(size_t num_elements) : sz(num_elements), cap(num_elements) {
        arr = new T[cap]{};
    }

    constexpr Vector(size_t num_elements, const T& val) : sz(num_elements), cap(num_elements) {
        arr = new T[cap];
        for (size_t i = 0; i < cap; i++)
            arr[i] = val;
    }

    constexpr Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        arr = new T[cap];
        for (size_t i = 0; i < sz; ++i)
            arr[i] = other.arr[i];
    }

    constexpr Vector(std::initializer_list<T> il) {
        sz = il.size();
        cap = sz;
        arr = new T[cap];
        size_t i = 0;
        for (const auto& elem : il) {
            arr[i++] = elem;
        }
    }

    constexpr Vector& operator=(std::initializer_list<T> il) {
        clear();
        sz = il.size();
        cap = sz;
        arr = new T[cap];
        size_t i = 0;
        for (const auto& elem : il) {
            arr[i++] = elem;
        }
        return *this;
    }

    constexpr Vector& operator=(const Vector& other) {
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

    constexpr void clear() {
        sz = 0;
    }

    constexpr Vector(Vector&& other) noexcept : arr(other.arr), sz(other.sz), cap(other.cap) {
        other.arr = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    constexpr Vector& operator=(Vector&& other) noexcept {
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

    constexpr void reserve(size_t newCapacity) {
        if (newCapacity > cap) {
            T* tmp = new T[newCapacity];
            for (size_t i = 0; i < sz; ++i)
                tmp[i] = std::move(arr[i]);
            delete[] arr;
            arr = tmp;
            cap = newCapacity;
        }
    }

    constexpr void resize(size_t newCapacity) {
        reserve(newCapacity);
        sz = newCapacity;
    }

    constexpr size_t size() const { return sz; }
    constexpr size_t capacity() const { return cap; }

    constexpr T& operator[](size_t i) { return arr[i]; }
    constexpr const T& operator[](size_t i) const { return arr[i]; }

    constexpr void push_back(const T& x) {
        if (sz == cap)
            reserve(cap > 0 ? cap * 2 : 1);
        arr[sz++] = x;
    }

    constexpr void pop_back() {
        if (sz > 0)
            --sz;
    }

    constexpr void insert(size_t pos, const T& x) {
        if (pos > sz) {
#if __cpp_constexpr_dynamic_alloc >= 201907L
            // constexpr dynamic alloc exists, so we can throw
            throw std::out_of_range("Vector::insert: position out of range");
#else
            __builtin_unreachable(); // constexpr safe error
#endif
        }
        if (sz == cap)
            reserve(cap > 0 ? cap * 2 : 1);
        for (size_t i = sz; i > pos; --i)
            arr[i] = std::move(arr[i - 1]);
        arr[pos] = x;
        ++sz;
    }

    struct iterator {
        T* ptr;

        constexpr iterator(T* p) : ptr(p) {}

        constexpr T& operator*() const { return *ptr; }
        constexpr T* operator->() const { return ptr; }

        constexpr iterator& operator++() {
            ++ptr;
            return *this;
        }

        constexpr iterator operator++(int) {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const { return ptr == other.ptr; }
        constexpr bool operator!=(const iterator& other) const { return ptr != other.ptr; }

        constexpr iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }
        constexpr iterator operator-(ptrdiff_t n) const { return iterator(ptr - n); }

        constexpr ptrdiff_t operator-(const iterator& other) const { return ptr - other.ptr; }
    };

    constexpr iterator begin() { return iterator(arr); }
    constexpr iterator end() { return iterator(arr + sz); }
    constexpr const iterator begin() const { return iterator(arr); }
    constexpr const iterator end() const { return iterator(arr + sz); }

    constexpr void insert(iterator it, const T& x) {
        size_t pos = static_cast<size_t>(it - begin());
        insert(pos, x);
    }

    constexpr bool empty() const { return sz == 0; }

    constexpr T* data() { return arr; }
    constexpr const T* data() const { return arr; }

    template<typename InputIt>
    constexpr void insert(iterator pos, InputIt first, InputIt last) {
        size_t insert_pos = static_cast<size_t>(pos - begin());
        size_t num_elements = static_cast<size_t>(last - first);

        if (sz + num_elements > cap) {
            reserve(std::max(cap * 2, sz + num_elements));
        }

        for (size_t i = sz; i > insert_pos; --i) {
            arr[i + num_elements - 1] = std::move(arr[i - 1]);
        }

        for (size_t i = 0; i < num_elements; ++i) {
            arr[insert_pos + i] = *(first + i);
        }

        sz += num_elements;
    }

        // Move push_back
        constexpr void push_back(T&& x) {
            if (sz == cap)
                reserve(cap > 0 ? cap * 2 : 1);
            arr[sz++] = std::move(x);
        }
    
        // Emplace_back
        template<typename... Args>
        constexpr T& emplace_back(Args&&... args) {
            if (sz == cap)
                reserve(cap > 0 ? cap * 2 : 1);
            new (&arr[sz]) T(std::forward<Args>(args)...);
            return arr[sz++];
        }
    
        // back() access
        constexpr T& back() {
            return arr[sz - 1];
        }
    
        constexpr const T& back() const {
            return arr[sz - 1];
        }
    

private:
    T* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
};

}  // namespace core

#endif // LIB_VECTOR_H
