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

#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace core {

MemMapFile::MemMapFile(const std::string& path) {
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ == -1) {
        throw FileOpenFailure();
    }

    off_t len = lseek(fd_, 0, SEEK_END);
    if (len == -1) {
        close(fd_);
        throw FileOpenFailure();
    }

    size_ = static_cast<size_t>(len);
    data_ = static_cast<char*>(mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0));

    if (data_ == MAP_FAILED) {
        close(fd_);
        throw FileOpenFailure();
    }
}

MemMapFile::~MemMapFile() {
    if (data_ && data_ != MAP_FAILED)
        munmap(const_cast<char*>(data_), size_);

    if (fd_ != -1)
        close(fd_);
}

}  // namespace core
