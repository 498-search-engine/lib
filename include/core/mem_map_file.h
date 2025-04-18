/**
 * @file mem_map_file.h
 * @author Christopher Davis
 * @brief RAII wrappe for memory mapped file
 * @version 0.1
 * @date 2025-04-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LIB_MEM_MAP_FILE_H
#define LIB_MEM_MAP_FILE_H

#include <string>
#include <stdexcept>

namespace core {

class FileOpenFailure final : public std::exception {
public:
    FileOpenFailure(const std::string& file, std::string msg = "")
        : what_("File open failure: failed to memory map file " + file + " " + msg) {}
    const char* what() const noexcept override {
        return what_.c_str();
    }
private:
    std::string what_;
};

class MemMapFile {
public:
    MemMapFile(const std::string& path);

    MemMapFile(const MemMapFile&) = delete;
    MemMapFile& operator=(const MemMapFile&) = delete;

    ~MemMapFile();

    const char* data() const noexcept { return data_; }
    size_t size() const noexcept { return size_; }

private:
    int fd_{-1};
    const char* data_{nullptr};
    size_t size_{0};
};

}  // namespace core

#endif
