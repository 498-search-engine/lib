#ifndef LIB_STRING_H
#define LIB_STRING_H

// string.h
/*
 * Our version of string.
 */
#include <cstddef>   // for size_t
#include <iostream>  // for ostream

static_assert(sizeof(size_t) == 8, "size_t is not 8 bytes");  // Ensure our union matches correctly

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
    // We restrict to only even capacity as a result which allows nice growth of strings
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
    String();

    // string Literal / C string Constructor
    // REQUIRES: cstr is a null terminated C style string
    // MODIFIES: *this
    // EFFECTS: Creates a string with equivalent contents to cstr
    String(const char* cstr);

    // Size
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns the number of characters in the string
    size_t Size() const;

    // Capacity
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns char size
    size_t Capacity() const;

    // C string Conversion
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a pointer to a null terminated C string of *this
    const char* Cstr() const;

    // Iterator Begin
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to the start of the string
    const char* begin() const;
    char* begin();

    // Iterator End
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns a random access iterator to the end of the string
    const char* end() const;

    // Element Access
    // REQUIRES: 0 <= i < size()
    // MODIFIES: Allows modification of the i'th element
    // EFFECTS: Returns the i'th character of the string
    char& operator[](size_t i);

    // string Append
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Appends the contents of other to *this, resizing any
    //      memory at most once
    void operator+=(const String& other);

    // Push Back
    // REQUIRES: Nothing
    // MODIFIES: *this
    // EFFECTS: Appends c to the string
    void PushBack(char c);

    // Pop Back
    // REQUIRES: string is not empty
    // MODIFIES: *this
    // EFFECTS: Removes the last charater of the string
    void PopBack();

    // Equality Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether all the contents of *this
    //    and other are equal
    bool operator==(const String& other) const;

    // Not-Equality Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether at least one character differs between
    //    *this and other
    bool operator!=(const String& other) const;

    // Less Than Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less than other
    bool operator<(const String& other) const;

    // Greater Than Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically greater than other
    bool operator>(const String& other) const;

    // Less Than Or Equal Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less or equal to other
    bool operator<=(const String& other) const;

    // Greater Than Or Equal Operator
    // REQUIRES: Nothing
    // MODIFIES: Nothing
    // EFFECTS: Returns whether *this is lexigraphically less or equal to other
    bool operator>=(const String& other) const;

    // Automatically grows the capacity of the string
    // REQUIRES: Nothing
    // MODIFIES: buffer, capacity
    // EFFECTS: all iterators invalidated
    void Grow();

    // Resizes the capacity of the string to *at least* new_capacity
    // no effect if new_capacity smaller in short string mode
    // REQUIRES: Nothing
    // MODIFIES: buffer, capacity
    // EFFECTS: all iterators invalidated
    void Resize(size_t new_capacity);

private:
    string_t internal_;

    // Mode of string
    // REQUIRES: Nothing
    // MODIFIES: nothing
    // EFFECTS: nothing
    bool IsShort() const;

    // Resizes the capacity of the string to straight up new_capacity
    // trusty resize function for internal use after grow
    // REQUIRES: new_capacity is a power of 2
    // MODIFIES: buffer, capacity
    // EFFECTS: forces heap string implementation after this
    void ResizePriv(size_t new_capacity);

    void ShortSetSize(size_t new_size);

    static size_t NextPower2(size_t n);
};

std::ostream& operator<<(std::ostream& os, const String& s);

#endif
