// vector.h
// 
// Starter file for a vector template
#include <utility>
#include <iostream>

template<typename T>
   class vector
   {
   public:

      // Default Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Constructs an empty vector with capacity 0
      vector( ) : arr(nullptr), sz(0), cap(0) {}

      // Destructor
      // REQUIRES: Nothing
      // MODIFIES: Destroys *this
      // EFFECTS: Performs any neccessary clean up operations
      ~vector( )
         {
            delete[] arr;
         }

      // Resize Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Constructs a vector with size num_elements,
      //    all default constructed
      vector( size_t num_elements ) : sz(num_elements), cap(num_elements)
         {
            arr = new T[cap]{};
         }

      // Fill Constructor
      // REQUIRES: Capacity > 0
      // MODIFIES: *this
      // EFFECTS: Creates a vector with size num_elements, all assigned to val
      vector( size_t num_elements, const T &val ) : sz(num_elements), cap(num_elements)
         {
            arr = new T[cap];
            for (int i = 0; i < cap; i++)
               arr[i] = val;
         }

      // Copy Constructor
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Creates a clone of the vector other
      vector( const vector<T> &other ) : sz(other.sz), cap(other.cap)
         {
            arr = new T[cap];
            for (size_t i = 0; i < sz; ++i)
               arr[i] = other.arr[i];
         }

      // Assignment operator
      // REQUIRES: Nothing
      // MODIFIES: *this
      // EFFECTS: Duplicates the state of other to *this
      vector& operator=( const vector<T> &other )
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
      // EFFECTS: Takes the data from other into a newly constructed vector
      vector( vector<T> &&other ) noexcept : arr(other.arr), sz(other.sz), cap(other.cap)
         {
            other.arr = nullptr;
            other.sz = 0;
            other.cap = 0;
         }

      // Move Assignment Operator
      // REQUIRES: Nothing
      // MODIFIES: *this, leaves other in a default constructed state
      // EFFECTS: Takes the data from other in constant time
      vector& operator=( vector<T> &&other )
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
      // EFFECTS: Ensures that the vector can contain size( ) = new_capacity
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

       void resize(size_t newCapacity){
        reserve(newCapacity);
        while (sz < cap) push_back(T());
       }
      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns the number of elements in the vector
      size_t size( ) const
         {
            return sz;
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns the maximum size the vector can attain before resizing
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
      // EFFECTS: Appends the element x to the vector, allocating
      //    additional space if neccesary
      void push_back( const T &x )
         {
            if (sz == cap)
               reserve(cap > 0 ? cap * 2 : 1);
            arr[sz] = x;
            sz++;
         }

      // REQUIRES: Nothing
      // MODIFIES: this, size( )
      // EFFECTS: Removes the last element of the vector,
      //    leaving capacity unchanged
      void pop_back( )
         {
            if (sz > 0) sz--;
         }

      // REQUIRES: Nothing
      // MODIFIES: Allows mutable access to the vector's contents
      // EFFECTS: Returns a mutable random access iterator to the 
      //    first element of the vector
      // T* begin( )
      //    {
      //       return arr;
      //    }

      // // REQUIRES: Nothing
      // // MODIFIES: Allows mutable access to the vector's contents
      // // EFFECTS: Returns a mutable random access iterator to 
      // //    one past the last valid element of the vector
      // T* end( )
      //    {
      //       return arr + size();
      //    }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns a random access iterator to the first element of the vector
      const T* begin( ) const
         {
            return arr;
         }

      // REQUIRES: Nothing
      // MODIFIES: Nothing
      // EFFECTS: Returns a random access iterator to 
      //    one past the last valid element of the vector
      const T* end( ) const
         {
            return arr + size();
         }

    class Iterator {
        private:
            T* ptr;
        public:
            explicit Iterator(T* p) : ptr(p) {}
            T& operator*() { return *ptr; }
            Iterator& operator++() { ++ptr; return *this; }
            Iterator operator+(uint32_t offset) {
               auto cpy = ptr;
               return Iterator(cpy + offset);
            }
            bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() { return Iterator(arr); }
    Iterator end() { return Iterator(arr + sz); }
   private:

      //TODO
      T* arr = nullptr;
      size_t sz = 0;
      size_t cap = 0;

   };
