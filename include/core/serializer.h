#ifndef LIB_SERIALIZER_H
#define LIB_SERIALIZER_H


#include <cstddef>  // for size_t
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>


namespace core {


template<typename T>
class AbstractSerializer {
public:
    virtual ~AbstractSerializer() = default;
    // create a buffer and serialize object to it however you want
    virtual char* serialize(const T& obj) const = 0;

    // deserialize from some T that was AbstractSerializer::serialize into that buffer
    virtual T deserialize(char* buffer, size_t size) const = 0;

    // how many bytes we need to serialize object T
    virtual size_t bytesRequired() const = 0;
};
};  // namespace core


class OBufferArchive {
public:
    static constexpr bool isLoading = false;

    OBufferArchive() = default;

    // Return a const reference to the underlying buffer
    const std::vector<char>& buffer() const { return buffer_; }

    // Alternatively, a pointer + size if needed
    const char* data() const { return buffer_.data(); }
    std::size_t size() const { return buffer_.size(); }

    // Clears the internal buffer
    void clear() { buffer_.clear(); }

    template<typename T>
    OBufferArchive& operator&(const T& value) {
        static_assert(std::is_arithmetic<T>::value,
                      "OBufferArchive only supports arithmetic by default here. "
                      "Use custom serialize(...) for complex types.");
        writeBytes(&value, sizeof(T));
        return *this;
    }

    // In OBufferArchive:
    OBufferArchive& operator&(const char* cstr) {
        // 1) Determine length (0 if cstr == nullptr)
        std::size_t length = 0;
        if (cstr) {
            length = std::strlen(cstr);
        }

        // 2) Write length as an arithmetic type
        *this& length;  // calls the arithmetic overload

        // 3) Write the raw characters if length > 0
        if (length > 0) {
            writeBytes(cstr, length);
        }

        return *this;
    }

    OBufferArchive& operator&(const std::string& str) {
        // 1) write length
        std::size_t length = str.size();

        *this& length;  // calls the arithmetic overload

        // 2) write the raw characters
        if (length > 0) {
            writeBytes(str.data(), length);
        }
        return *this;
    }

private:
    std::vector<char> buffer_;

    void writeBytes(const void* ptr, std::size_t size) {
        const char* cptr = static_cast<const char*>(ptr);
        buffer_.insert(buffer_.end(), cptr, cptr + size);
    }
};

class IBufferArchive {
public:
    static constexpr bool isLoading = true;

    // The constructor takes a pointer to an existing buffer and its length.
    IBufferArchive(const char* data, std::size_t size) : data_(data), size_(size), pos_(0) {}

    // No copying to avoid confusion, but you can allow it if you like
    IBufferArchive(const IBufferArchive&) = delete;
    IBufferArchive& operator=(const IBufferArchive&) = delete;

    // -----------------------------
    // operator& for arithmetic T
    // -----------------------------
    template<typename T>
    IBufferArchive& operator&(T& value) {
        static_assert(std::is_arithmetic<T>::value,
                      "IBufferArchive only supports arithmetic by default. "
                      "Use custom serialize(...) for non-trivial types.");
        readBytes(&value, sizeof(T));
        return *this;
    }

    // -----------------------------
    // operator& for std::string
    // -----------------------------
    IBufferArchive& operator&(std::string& str) {
        // 1) read length
        std::size_t length = 0;
        *this& length;
        // 2) read the characters
        str.clear();
        if (length > 0) {
            str.resize(length);
            readBytes(&str[0], length);
        }
        return *this;
    }

    IBufferArchive& operator&(const char*& str) {
        // 1) read the length
        std::size_t length = 0;
        *this& length;  // calls the arithmetic overload

        // 2) if there's data, allocate + read
        if (length > 0) {
            // allocate a buffer, read the bytes
            char* temp = new char[length + 1];
            readBytes(temp, length);
            temp[length] = '\0';  // null-terminate
            str = temp;           // store the pointer in 'str'
        } else {
            // no data means make str null
            str = nullptr;
        }
        return *this;
    }

private:
    const char* data_{nullptr};
    std::size_t size_{0};
    std::size_t pos_{0};

    void readBytes(void* dst, std::size_t len) {
        if (pos_ + len > size_) {
            throw std::runtime_error("IBufferArchive: out of range read");
        }
        std::memcpy(dst, data_ + pos_, len);
        pos_ += len;
    }
};


#endif