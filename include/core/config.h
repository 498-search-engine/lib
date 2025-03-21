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

        if (bytesRead < 0) {
            throw std::runtime_error("Error reading config file");
        }

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
        return (it != config_map_.end()) ? std::stoi(it->second.Cstr()) : default_value;
    }

    int GetInt(const String &key) const {
        auto it = config_map_.find(key);
        if (it == config_map_.end()) {
            const String error = "key " + key + " not found";
            throw std::invalid_argument(error.Cstr());
        }

        return std::stoi(it->second.Cstr());
    }

    private:
        std::unordered_map<String, String> config_map_; 

        void ProcessLine(const String &key, const char *line, size_t end) {
            config_map_[key] = String(line, end);
        }

        void Parse(const String &content) {
            const char *data = content.Cstr();
            size_t lineStart = 0;
            size_t i = 0;
            size_t len = content.Size();
        
            while (i <= len) {
                if (data[i] == '\n' || data[i] == '\0') {
                    size_t lineLen = (i > lineStart && data[i - 1] == '\r') ? i - lineStart - 1 : i - lineStart;
                    if (lineLen > 0) {
                        String line(data + lineStart, lineLen);
                        line.Trim();
        
                        if (!line.Empty() && line.Cstr()[0] != '#') {
                            size_t colonPos = line.Find( ':');
                            if (colonPos != String::Npos) {
                                String key = line;
                                key.Substr(0, colonPos);
                                key.Trim();

                                String value = line;
                                value.Substr(colonPos + 1);
                                value.Trim();

                                ProcessLine(key, value.Cstr(), value.Size());
                            }
                        }
                    }
                    lineStart = i + 1;
                }
                i++;
            }
        }
};

} // namespace core
#endif
