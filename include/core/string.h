#ifndef LIB_STRING_H
#define LIB_STRING_H

// string.h
//
// Starter file for a string template

#include <cstddef>   // for size_t
#include <iostream>  // for ostream

static_assert(sizeof(size_t) == 8, "size_t is not 8 bytes");  // Ensure our union matches correctly

namespace core {

inline constexpr int StackStringSize = 22;

/*
   Size of char array including null terminator
*/
inline constexpr int StackCharSize = StackStringSize + 1;

/*
   Size of total stack char array
*/
inline constexpr int StackArrSize = StackCharSize + 1;

inline constexpr unsigned int RepFlagMask = 1;

class String {
    struct HeapStringT {
        char* ptr;        // 8 bytes on 64-bit systems
        size_t size;      // 8 bytes
        size_t capacity;  // 8 bytes
    };

    // NOTE: should we allow shrinking to stack_string from heap_string?
    // NOTE: Theoretical optimization to consider
    // storing size_left instead would allow you to store 23 bytes excluding null terminator (since size_left = 0 = \0
    // when string is full)
    struct StackStringT {
        char data[StackArrSize];  // 23 bytes (including null terminator)
                                  // + 1 byte at end for SIZE
                                  // a seperate size isn't added to force struct padding to be nice.
    };

    // Last bit in each case represents whether we're in short or long string mode
    // We restrict to only even capacity as a result which allows nice Growth of strings
    // and allows us to store this much data in the string.
    union string_t {
        HeapStringT heap_string;
        StackStringT stack_string;
    };

public:
    // Default Constructor
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Creates an empty string
    String() {
        for (size_t i = 0; i < StackArrSize; ++i) {
            internal_.stack_string.data[i] = 0;
        }
        // memset(internal_.stack_string.data, 0, StackArrSize);
        internal_.stack_string.data[StackCharSize] = 1;
    }
    // string Literal / C string Constructor
    // REQUIRES: cstr is a null terminated C style string
    // MODIFIES: *this
    // EFFECTS: Creates a string with equivalent contents to cstr
    String(const char* cstr) {
        // set it as short string
        for (size_t i = 0; i < StackArrSize; ++i) {
          internal_.stack_string.data[i] = 0;
        }
        // memset(internal_.stack_string.data, 0, StackArrSize);
        internal_.stack_string.data[StackCharSize] = 1;

        char *buf = internal_.stack_string.data;

        // avoid calls to size() & Capacity() as we already know this is a short string at the beginning in constructor
        size_t ssize = 0;
        size_t scapacity = StackStringSize;

        while (*cstr != 0) {
            if (ssize == scapacity) {
                Grow();
                buf = internal_.heap_string.ptr + ssize;
                scapacity = Capacity();
            }

            *buf = *cstr;

            buf++;
            cstr++;
            ssize++;
        }
        
        if (scapacity > StackStringSize) {
            // heap string
            internal_.heap_string.size = ssize;
        } else {
            // short string
            ShortSetSize(ssize);
        }
    }

    String(const char* cstr, size_t n) : String() {
        Resize(n);

        // stack allocate string
        if (IsShort()) {
            for (size_t i = 0; i < n; ++i) {
                internal_.stack_string.data[i] = cstr[i];
            }

            ShortSetSize(n);
            return;
        }

        // heap allocate string
        for (size_t i = 0; i < n; ++i) {
            internal_.heap_string.ptr[i] = cstr[i];
        }

        internal_.heap_string.size = n;
    }

    ~String() {
        if (!IsShort()) {
            delete[] internal_.heap_string.ptr;
        }
    };

    // Copy constructor
    String(const String& other) : String(other.Cstr(), other.Size()) {}

    // Copy assignment
    String& operator=(const String &other) {
        if (this == &other) {
            return *this;
        }
        
        const char *cstr = other.Cstr();
        size_t n = other.Size();

        Resize(n);

        // stack allocate string
        if (IsShort()) {
            for (size_t i = 0; i < n; ++i) {
                internal_.stack_string.data[i] = cstr[i];
            }

            ShortSetSize(n);
            return *this;
        }

        // heap allocate string
        for (size_t i = 0; i < n; ++i) {
            internal_.heap_string.ptr[i] = cstr[i];
        }

        internal_.heap_string.size = n;

        return *this;
    }

    // Move constructor
    String(String &&other) noexcept {
        if (other.IsShort()) {
            for (size_t i = 0; i < StackArrSize; ++i) {
                this->internal_.stack_string.data[i] = other.internal_.stack_string.data[i];
            }

            other.Nullify();
            return;
        }

        this->internal_.heap_string.ptr = other.internal_.heap_string.ptr;
        this->internal_.heap_string.capacity = other.internal_.heap_string.capacity;
        this->internal_.heap_string.size = other.internal_.heap_string.size;

        other.Nullify();
    }

    // Move assignment constructor
    String& operator=(String &&other) noexcept {
        if (other.IsShort()) {
            for (size_t i = 0; i < StackArrSize; ++i) {
                this->internal_.stack_string.data[i] = other.internal_.stack_string.data[i];
            }

            other.Nullify();

            return *this;
        }

        this->internal_.heap_string.ptr = other.internal_.heap_string.ptr;
        this->internal_.heap_string.capacity = other.internal_.heap_string.capacity;
        this->internal_.heap_string.size = other.internal_.heap_string.size;

        other.Nullify();

        return *this;
    }

    // Size
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the number of characters in the string
    size_t Size() const {
        if (IsShort()) {
            return internal_.stack_string.data[StackCharSize] >> 1;
        }

        return internal_.heap_string.size;
    }

    // Capacity
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns char size
    size_t Capacity() const {
        if (IsShort()) {
            return StackStringSize;
        }

        return internal_.heap_string.capacity;
    }

    // C string Conversion
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a pointer to a null terminated C string of *this
    const char* Cstr() const {
        if (IsShort()) {
            return internal_.stack_string.data;
        }
        return internal_.heap_string.ptr;
    }

    // Iterator Begin
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to the start of the string
    const char* begin() const {
        return Cstr();
    }

    char* begin() {
        if (IsShort()) {
            return internal_.stack_string.data;
        }

        return internal_.heap_string.ptr;
    }

    // Iterator End
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to the end of the string
    const char* end() const {
        return Cstr() + Size();
    }

    // Element Access
    // REQUIRES: 0 <= i < size()
    // MODIFIES: Allows modification of the i'th element
    // EFFECTS: Returns the i'th character of the string
    char& operator[](size_t i) {
        if (IsShort()) {
            return internal_.stack_string.data[i];
        }

        return internal_.heap_string.ptr[i];
    }

    // string Append
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Appends the contents of other to *this, resizing any
    //      memory at most once
    void operator+=(const String& other) {
        // Grow string if necessary
        const size_t currentSize = Size();
        size_t newSize = currentSize + other.Size();
        if (Capacity() < newSize) {
            Resize(newSize);
        }

        // Copy over string
        const auto *it = other.begin();
        const auto *end = other.end();

        char *currIt = begin() + currentSize;
        while (it != end) {
            *currIt = *it;
            currIt++;
            it++;
        }

        // String size update
        if (IsShort()) {
            ShortSetSize(newSize);
        } else {
            internal_.heap_string.size = newSize;
        }
    }

    // Push Back
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Appends c to the string
    void PushBack(char c) {
        const size_t ssize = Size();
        if (Capacity() == ssize) {
            Grow();
        }

        if (IsShort()) {
            internal_.stack_string.data[ssize] = c;
            ShortSetSize(ssize + 1);
        } else {
            internal_.heap_string.ptr[ssize] = c;
            internal_.heap_string.size = ssize + 1;
        }
    }

    // Pop Back
    // REQUIRES: string is not empty
    // MODIFIES: *this
    // EFFECTS: Removes the last charater of the string
    void PopBack() {
        const size_t ssize = Size();
        // assert(ssize > 0);
        
        if (IsShort()) {
            internal_.stack_string.data[ssize - 1] = '\0';
            ShortSetSize(ssize - 1);
        } else {
            internal_.heap_string.ptr[ssize - 1] = '\0';
            internal_.heap_string.size = ssize - 1;
        }    
    }

    // Equality Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether all the contents of *this
    //    and other are equal
    bool operator==(const String& other) const {
        // quick check for diff string size
        if (Size() != other.Size()) {
            return false;
        }

        const char *curBuf = begin();
        const char *otherBuf = other.begin();

        const char *curBufEnd = end();
        const char *otherBufEnd = other.end();

        while (curBuf != curBufEnd && otherBuf != otherBufEnd) {
            if (*curBuf != *otherBuf) {
                return false;
            }
            curBuf++;
            otherBuf++;
        }

        return curBuf == curBufEnd && otherBuf == otherBufEnd;
    }

    // Not-Equality Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether at least one character differs between
    //    *this and other
    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    // Less Than Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less than other
    bool operator<(const String& other) const {
        const char *curBuf = begin();
        const char *otherBuf = other.begin();

        const char *curBufEnd = end();
        const char *otherBufEnd = other.end();

        while (curBuf != curBufEnd && otherBuf != otherBufEnd) {
            if (*curBuf != *otherBuf) {
                return *curBuf < *otherBuf;
            }
            curBuf++;
            otherBuf++;
        }

        return Size() < other.Size();
    }

    // Greater Than Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically greater than other
    bool operator>(const String& other) const {
        const char *curBuf = begin();
        const char *otherBuf = other.begin();

        const char *curBufEnd = end();
        const char *otherBufEnd = other.end();

        while (curBuf != curBufEnd && otherBuf != otherBufEnd) {
            if (*curBuf != *otherBuf) {
                return *curBuf > *otherBuf;
            }
            curBuf++;
            otherBuf++;
        }

        return Size() > other.Size();
    }

    // Less Than Or Equal Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less or equal to other
    bool operator<=(const String& other) const {
        const char *curBuf = begin();
        const char *otherBuf = other.begin();

        const char *curBufEnd = end();
        const char *otherBufEnd = other.end();

        while (curBuf != curBufEnd && otherBuf != otherBufEnd) {
            if (*curBuf != *otherBuf) {
                return *curBuf < *otherBuf;
            }
            curBuf++;
            otherBuf++;
        }

        return Size() <= other.Size();
    }

    // Greater Than Or Equal Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less or equal to other
    bool operator>=(const String& other) const {
        const char *curBuf = begin();
        const char *otherBuf = other.begin();

        const char *curBufEnd = end();
        const char *otherBufEnd = other.end();

        while (curBuf != curBufEnd && otherBuf != otherBufEnd) {
            if (*curBuf != *otherBuf) {
                return *curBuf > *otherBuf;
            }
            curBuf++;
            otherBuf++;
        }

        return Size() >= other.Size();
    }

    // Automatically Grows the capacity of the string
    // REQUIRES: Nothing
    // MODIFIES: buffer, capacity
    // EFFECTS: all iterators invalidated
    void Grow() {
        if (IsShort()) {
            // If it's stack allocated
            // start out with 64 to ensure evenness
            // arbitrary choice to double from more than 22 (to achieve amortized O(1))
            ResizePriv(64);
        } else {
            // If it's heap allocated
            // double size to ensure even-ness
            ResizePriv(internal_.heap_string.capacity * 2);
        }
    }

    // Resizes the capacity of the string to *at least* new_capacity
    // no effect if new_capacity smaller in short string mode
    // REQUIRES: Nothing
    // MODIFIES: buffer, capacity
    // EFFECTS: all iterators invalidated
    void Resize(size_t new_capacity) {
        // this is for safety in regards to creating the char * ptr and failing gracefully
        if (IsShort() && new_capacity < 22) {
            // Cannot make short representation smaller
            return;
        }

        // assert(new_capacity > 0);
        // round up to the next power of 2
        new_capacity = NextPower2(new_capacity);
        ResizePriv(new_capacity);
    }

private:
    string_t internal_;

    // Mode of string
    // REQUIRES: Nothing
    // MODIFIES: nothing
    // EFFECTS: nothing
    bool IsShort() const {
        return (internal_.stack_string.data[StackCharSize] & RepFlagMask) == 1;
    }

    // Resizes the capacity of the string to straight up new_capacity
    // trusty resize function for internal use after Grow
    // REQUIRES: new_capacity is a power of 2
    // MODIFIES: buffer, capacity
    // EFFECTS: forces heap string implementation after this
    void ResizePriv(size_t new_capacity) {
        // Allocate new memory for it
        // the +1 for nullcharacter
        char *newBuffer = new char[new_capacity + 1]{};

        // Copy over old stuff
        if (IsShort()) {
            // copy first 23 characters including null character
            for (size_t i = 0; i < StackCharSize; ++i) {
              newBuffer[i] = internal_.stack_string.data[i];
            }
            // memcpy(newBuffer, internal_.stack_string.data, StackCharSize);

            const size_t oldSize = Size();
        
            internal_.heap_string.ptr = newBuffer;
            internal_.heap_string.capacity = new_capacity;
            internal_.heap_string.size = oldSize;
        } else {
            if (internal_.heap_string.capacity < new_capacity) {
                // Grows
                for (size_t i = 0; i < internal_.heap_string.capacity; ++i) {
                  newBuffer[i] = internal_.heap_string.ptr[i];
                }
                // memcpy(newBuffer, internal_.heap_string.ptr, internal_.heap_string.capacity); 
        
            } else {
                // Shrinks
                for (size_t i = 0; i < new_capacity; ++i) {
                  newBuffer[i] = internal_.heap_string.ptr[i];
                }

                // memcpy(newBuffer, internal_.heap_string.ptr, new_capacity); 

                // Set ending to \0
                *(newBuffer + new_capacity) = '\0';

                // Shrink size if necessary
                if (internal_.heap_string.size > internal_.heap_string.capacity) {
                    internal_.heap_string.size = internal_.heap_string.capacity;
                }
            }

            delete[] internal_.heap_string.ptr;
            internal_.heap_string.ptr = newBuffer;
            internal_.heap_string.capacity = new_capacity;
        }
    }

    inline void ShortSetSize(size_t new_size) {
        // keeps the last bit as 1 to maintain short string notation
        // this conversion is fine since new_size < 22
        internal_.stack_string.data[StackCharSize] = (new_size << 1) + 1;
    }

    void Nullify() {
        for (size_t i = 0; i < StackCharSize; ++i) {
            internal_.stack_string.data[i] = 0;
        }

        internal_.stack_string.data[StackCharSize] = 1;
    }

    static size_t NextPower2(size_t n) {
        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n |= n >> 32;
        ++n;
        return n;
    }
};

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.Cstr();
    return os;
}

} // namespace core

#endif
