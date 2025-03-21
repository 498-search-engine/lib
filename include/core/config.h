#ifndef LIB_CUSTOM_CONFIG_H
#define LIB_CUSTOM_CONFIG_H

#include <core/string.h>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <unordered_map>

#define BUFFER_SIZE 1024

namespace core {

class Config {
    public:
    Config(const String &file) {
        const String targetFile = "config/" + file;

        int fd = open(targetFile.Cstr(), O_RDONLY);
        if (fd == -1) {
            const String error = "failed to open file: " + targetFile;
            throw std::invalid_argument(error.Cstr());
        }

        char buffer[BUFFER_SIZE];
        String content;
        ssize_t bytesRead;

        while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            content += buffer;
        }

        close(fd);

        Parse(content);
    };

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    String GetString(const String &key, const String &default_value) const {
        auto it = config_map_.find(key);
        return (it != config_map_.end()) ? it->second : default_value;
    }

    String GetString(const String &key) const {
        auto it = config_map_.find(key);
        if (it == config_map_.end()) {
            const String error = "key " + key + " not found";
            throw std::invalid_argument(error.Cstr());
        }

        return it->second;
    }

    int GetInt(const String &key, int default_value) const {
        auto it = config_map_.find(key);
        return (it != config_map_.end()) ? atoi(it->second.Cstr()) : default_value;
    }

    int GetInt(const String &key) const {
        auto it = config_map_.find(key);
        if (it == config_map_.end()) {
            const String error = "key " + key + " not found";
            throw std::invalid_argument(error.Cstr());
        }

        return atoi(it->second.Cstr());
    }

    private:
        std::unordered_map<String, String> config_map_; 

        void ProcessLine(const String &key, const char *line, size_t end) {
            std::cout << "adding key " << key << " to " << String(line, end) << std::endl;;
            config_map_[key] = String(line, end);
        }

        void Parse(const String &content) {
            const char *data = content.Cstr();
            size_t begin = 0;
            size_t end = 0;
            bool readingKey = true;
            bool hasDataStarted = false;
            String key = "";
        
            while (data[end] != '\0') {
                char ch = data[end];
        
                if (ch == '\n') {
                    if (!key.Empty()) {
                        size_t valueLength = end - begin;
                        if (valueLength > 0 && data[begin + valueLength - 1] == '\r') {
                            valueLength--;
                        }
                        ProcessLine(key, content.Cstr() + begin, valueLength);
                        key = "";
                    }
                    begin = end + 1;
                    readingKey = true;
                    hasDataStarted = false;
                } else if (ch == ':' && readingKey) {
                    key = String(content.Cstr() + begin, end - begin);
                    begin = end + 1;
                    readingKey = false;
                    hasDataStarted = false;
                } else if (ch == '#' && readingKey) {
                    while (data[end] != '\0' && data[end] != '\n') {
                        end++;
                    }
                    begin = end + 1;
                    key = "";
                    readingKey = true;
                    hasDataStarted = false;
                }
        
                if (ch != ' ' && ch != '\t' && !hasDataStarted) {
                    hasDataStarted = true;
                }
        
                end++;
            }
        
            // Handle last line if file does not end with a newline
            if (!key.Empty() && end > begin) {
                ProcessLine(key, content.Cstr() + begin, end - begin);
            }
        }
};

} // namespace core
#endif
