#ifndef CORE_VECTOR_FILE_H
#define CORE_VECTOR_FILE_H

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>

namespace core {

constexpr size_t PageSize = 4096;

/**
 * @brief VectorFile is a vector-like data structure that is backed by a
 * memory-mapped file. It supports O(1) random access, push back, and pop back.
 * Insertions and removals at arbitrary indicies are not permitted.
 *
 * @tparam T Vector element type
 */
template<typename T>
class VectorFile {
    struct FileHeader {
        size_t capacity;
        size_t size;
    };

    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to persist to disk");
    static_assert(std::has_unique_object_representations_v<FileHeader>,
                  "FileHeader must have a unique object representation (no padding)");

    // Round sizeof(FileHeader) up to multiple of alignof(T)
    static constexpr size_t FileHeaderSpace = (sizeof(FileHeader) + alignof(T) - 1) & ~(alignof(T) - 1);
    static_assert(sizeof(FileHeader) <= FileHeaderSpace);

public:
    static constexpr size_t EntriesPerPage = PageSize / sizeof(T);
    static constexpr size_t InitialCapacity = EntriesPerPage;

    /**
     * @brief Creates or opens a VectorFile at the given path.
     *
     * @param path Path of backing file
     */
    VectorFile(const char* path) {
        bool exists = access(path, F_OK) != -1;

        // Create/open file
        fd_ = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd_ == -1) {
            throw std::runtime_error("failed to open file");
        }

        if (exists) {
            // Get size of file in bytes
            struct stat st {};
            if (fstat(fd_, &st) == -1) {
                close(fd_);
                throw std::runtime_error("failed to get file size");
            }
            file_size_ = st.st_size;
        } else {
            // Initialize file
            file_size_ = FileHeaderSpace + (InitialCapacity * sizeof(T));
            if (ftruncate(fd_, file_size_) == -1) {
                close(fd_);
                throw std::runtime_error("failed to initialize file");
            }
        }

        // Map file into memory
        mapped_ = mmap(nullptr, file_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
        if (mapped_ == MAP_FAILED) {
            close(fd_);
            throw std::runtime_error("failed to map file");
        }

        if (!exists) {
            // Initialize header data
            Header()->capacity = InitialCapacity;
            Header()->size = 0;
        }

        capacity_ = Header()->capacity;
        size_ = Header()->size;
    }

    ~VectorFile() {
        if (mapped_ != nullptr) {
            munmap(mapped_, file_size_);
            mapped_ = nullptr;
        }
        if (fd_ != -1) {
            close(fd_);
            fd_ = -1;
        }
    }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }
    bool Empty() const { return size_ == 0; }

    T* Data() { return reinterpret_cast<T*>(static_cast<char*>(mapped_) + FileHeaderSpace); }
    const T* Data() const { return reinterpret_cast<const T*>(static_cast<char*>(mapped_) + FileHeaderSpace); }

    T& operator[](size_t n) {
        if (n >= size_) {
            throw std::out_of_range("index out of range");
        }
        return Data()[n];
    }

    const T& operator[](size_t n) const {
        if (n >= size_) {
            throw std::out_of_range("index out of range");
        }
        return Data()[n];
    }

    T& Front() { return (*this)[0]; }
    T& Back() { return (*this)[size_ - 1]; }

    T* begin() { return Data(); }
    T* end() { return Data()[size_]; }
    const T* begin() const { return Data(); }
    const T* end() const { return Data()[size_]; }

    void Reserve(size_t capacity) {
        if (capacity_ >= capacity) {
            return;
        }
        ForceResize(capacity);
    }

    /**
     * @brief Pushes the value to the end of the vector. May invalidate pointers
     * and iterators.
     *
     * @param value Value to push
     */
    void PushBack(const T& value) {
        if (size_ >= capacity_) {
            ForceResize(capacity_ * 2);
        }
        Data()[size_] = value;
        ++size_;
        Header()->size = size_;
    }

    /**
     * @brief Pops the value at the end of the vector off. May invalidate
     * pointers and iterators.
     */
    void PopBack() {
        if (size_ == 0) {
            throw std::out_of_range("index out of range");
        }

        --size_;
        Header()->size = size_;

        if (size_ < capacity_ / 4 && capacity_ > InitialCapacity) {
            ForceResize(size_ / 2);
        }
    }

private:
    FileHeader* Header() const { return static_cast<FileHeader*>(mapped_); }

    void ForceResize(size_t new_capacity) {
        if (mapped_ != nullptr) {
            munmap(mapped_, file_size_);
        }

        size_t new_file_size = FileHeaderSpace + (new_capacity * sizeof(T));
        if (ftruncate(fd_, static_cast<off_t>(new_file_size)) == -1) {
            throw std::runtime_error("failed to resize file");
        }

        mapped_ = mmap(nullptr, new_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
        if (mapped_ == MAP_FAILED) {
            throw std::runtime_error("failed to remap file after resize");
        }

        file_size_ = new_file_size;
        capacity_ = new_capacity;
        Header()->capacity = new_capacity;
    }

    int fd_{-1};
    void* mapped_{nullptr};
    size_t file_size_{0};  // backing file size

    size_t capacity_{0};  // vector capacity
    size_t size_{0};      // vector size
};


}  // namespace core

#endif
