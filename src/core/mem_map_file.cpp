/**
 * @file mem_map_file.cpp
 * @author Christopher Davis
 * @brief Memory mapped file RAII wrapper
 * @version 0.1
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core/mem_map_file.h"

#include <fcntl.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

namespace core {

MemMapFile::MemMapFile(const std::string& path, bool forceInMemory) {
    std::cout << "in memmap file constructor " << path;
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ == -1) {
        throw FileOpenFailure(path, "bad fd");
    }

    off_t len = lseek(fd_, 0, SEEK_END);
    if (len == -1) {
        close(fd_);
        throw FileOpenFailure(path, "bad length");
    }

    size_ = static_cast<size_t>(len);
    if (size_ == 0) {
        close(fd_);
        throw FileOpenFailure(path, "size zero");
    }

    data_ = static_cast<char*>(mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0));

    if (data_ == MAP_FAILED) {
        close(fd_);
        throw FileOpenFailure(path, "mmap() failed");
    }

    if (forceInMemory) {
        if (mlock(data_, size_) != 0) {
            std::cout << "unable to mlock " << path;
        }

        // Touch all pages to force in memory
        volatile char tmp;
        for (size_t i = 0; i < size_; i += 4096) {
            tmp = *((char*)data_ + i);
        }
    }
}

MemMapFile::~MemMapFile() {
    if (data_ && data_ != MAP_FAILED)
        munmap(const_cast<char*>(data_), size_);

    if (fd_ != -1)
        close(fd_);
}

}  // namespace core
