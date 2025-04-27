// vector.h
//
// Starter file for a vector template
#include <iostream>
#include <utility>

template<typename T>
class vector {
public:
    // Default Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Constructs an empty vector with capacity 0
    vector() : arr(nullptr), sz(0), cap(0) {}

    // Destructor
    // REQUIRES: Nothing
    // MODIFIES: Destroys *this
    // EFFECTS: Performs any neccessary clean up operations
    ~vector() { delete[] arr; }

    // Resize Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Constructs a vector with size num_elements,
    //    all default constructed
    vector(size_t num_elements) : sz(num_elements), cap(num_elements) { arr = new T[cap]{}; }

    // Fill Constructor
    // REQUIRES: Capacity > 0
    // MODIFIES: *this
    // EFFECTS: Creates a vector with size num_elements, all assigned to val
    vector(size_t num_elements, const T& val) : sz(num_elements), cap(num_elements) {
        arr = new T[cap];
        for (int i = 0; i < cap; i++)
            arr[i] = val;
    }

    // Copy Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Creates a clone of the vector other
    vector(const vector<T>& other) : sz(other.sz), cap(other.cap) {
        arr = new T[cap];
        for (size_t i = 0; i < sz; ++i)
            arr[i] = other.arr[i];
    }

    // Assignment operator
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Duplicates the state of other to *this
    vector& operator=(const vector<T>& other) {
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

    // Move Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this, leaves other in a default constructed state
    // EFFECTS: Takes the data from other into a newly constructed vector
    vector(vector<T>&& other) noexcept : arr(other.arr), sz(other.sz), cap(other.cap) {
        other.arr = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Move Assignment Operator
    // REQUIRES: Nothing
    // MODIFIES: *this, leaves other in a default constructed state
    // EFFECTS: Takes the data from other in constant time
    vector& operator=(vector<T>&& other) {
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
    // EFFECTS: Ensures that the vector can contain size( ) = new_capacity
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

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the number of elements in the vector
    size_t size() const { return sz; }

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the maximum size the vector can attain before resizing
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
    // EFFECTS: Appends the element x to the vector, allocating
    //    additional space if neccesary
    void pushBack(const T& x) {
        if (sz == cap)
            reserve(cap > 0 ? cap * 2 : 1);
        arr[sz] = x;
        sz++;
    }

    // REQUIRES: Nothing
    // MODIFIES: this, size( )
    // EFFECTS: Removes the last element of the vector,
    //    leaving capacity unchanged
    void popBack() {
        if (sz > 0)
            sz--;
    }

    // REQUIRES: Nothing
    // MODIFIES: Allows mutable access to the vector's contents
    // EFFECTS: Returns a mutable random access iterator to the
    //    first element of the vector
    T* begin() { return arr; }

    // REQUIRES: Nothing
    // MODIFIES: Allows mutable access to the vector's contents
    // EFFECTS: Returns a mutable random access iterator to
    //    one past the last valid element of the vector
    T* end() { return arr + size(); }

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to the first element of the vector
    const T* begin() const { return arr; }

    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to
    //    one past the last valid element of the vector
    const T* end() const { return arr + size(); }

private:
    // TODO
    T* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
};
#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

// Vector.h
// 
// Starter file for a Vector template
#include <utility>
#include <iostream>
namespace core{


template<typename T>
   class Vector
   {
   public:

      // Default Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Constructs an empty Vector with capacity 0
      Vector( ) : arr(nullptr), sz(0), cap(0) {}

      // Destructor
      // REQUIRES: Nothing
      // MODIFIES: Destroys *this
      // EFFECTS: Performs any neccessary clean up operations
      ~Vector( )
         {
            delete[] arr;
         }

      // Resize Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Constructs a Vector with size num_elements,
      //    all default constructed
      Vector( size_t num_elements ) : sz(num_elements), cap(num_elements)
         {
            arr = new T[cap]{};
         }

      // Fill Constructor
      // REQUIRES: Capacity > 0
      // MODIFIES: *this
      // EFFECTS: Creates a Vector with size num_elements, all assigned to val
      Vector( size_t num_elements, const T &val ) : sz(num_elements), cap(num_elements)
         {
            arr = new T[cap];
            for (int i = 0; i < cap; i++)
               arr[i] = val;
         }

      // Copy Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Creates a clone of the Vector other
      Vector( const Vector<T> &other ) : sz(other.sz), cap(other.cap)
         {
            arr = new T[cap];
            for (size_t i = 0; i < sz; ++i)
               arr[i] = other.arr[i];
         }

      // Assignment operator
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Duplicates the state of other to *this
      Vector& operator=( const Vector<T> &other )
         {
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

      // Move Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this, leaves other in a default constructed state
      // EFFECTS: Takes the data from other into a newly constructed Vector
      Vector( Vector<T> &&other ) noexcept : arr(other.arr), sz(other.sz), cap(other.cap)
         {
            other.arr = nullptr;
            other.sz = 0;
            other.cap = 0;
         }

      // Move Assignment Operator
      // REQUIRES: Nothing
      // MODIFIES: *this, leaves other in a default constructed state
      // EFFECTS: Takes the data from other in constant time
      Vector& operator=( Vector<T> &&other )
         {
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
      void reserve( size_t newCapacity )
         {
            if (newCapacity > cap) {
               T* tmp = new T[newCapacity];
               for (size_t i = 0; i < sz; i++)
                  tmp[i] = std::move(arr[i]);
               delete[] arr;
               arr = tmp;
               cap = newCapacity;
            }
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns the number of elements in the Vector
      size_t size( ) const
         {
            return sz;
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns the maximum size the Vector can attain before resizing
      size_t capacity( ) const
         {
            return cap;
         }

      // REQUIRES: 0 <= i < size( )
      // MODIFIES: Allows modification of data[i]
      // EFFECTS: Returns a mutable reference to the i'th element
      T &operator[ ]( size_t i )
         {
            return arr[i];
         }

      // REQUIRES: 0 <= i < size( )
      // MODIFIES: Nothing
      // EFFECTS: Get a const reference to the ith element
      const T &operator[ ]( size_t i ) const
         {
            return arr[i];
         }

      // REQUIRES: Nothing
      // MODIFIES: this, size( ), capacity( )
      // EFFECTS: Appends the element x to the Vector, allocating
      //    additional space if neccesary
      void pushBack( const T &x )
         {
            if (sz == cap)
               reserve(cap > 0 ? cap * 2 : 1);
            arr[sz] = x;
            sz++;
         }

      // REQUIRES: Nothing
      // MODIFIES: this, size( )
      // EFFECTS: Removes the last element of the Vector,
      //    leaving capacity unchanged
      void popBack( )
         {
            if (sz > 0) sz--;
         }

      // REQUIRES: Nothing
      // MODIFIES: Allows mutable access to the Vector's contents
      // EFFECTS: Returns a mutable random access iterator to the 
      //    first element of the Vector
      T* begin( )
         {
            return arr;
         }

      // REQUIRES: Nothing
      // MODIFIES: Allows mutable access to the Vector's contents
      // EFFECTS: Returns a mutable random access iterator to 
      //    one past the last valid element of the Vector
      T* end( )
         {
            return arr + size();
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns a random access iterator to the first element of the Vector
      const T* begin( ) const
         {
            return arr;
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns a random access iterator to 
      //    one past the last valid element of the Vector
      const T* end( ) const
         {
            return arr + size();
         }

   private:

      //TODO
      T* arr = nullptr;
      size_t sz = 0;
      size_t cap = 0;

   };
}

#endif
