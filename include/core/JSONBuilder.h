#ifndef LIB_JSONBUILDER_H
#define LIB_JSONBUILDER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace core {

struct JSONBuilder {
    public:
    JSONBuilder(const std::string& str){
        if (str.empty()) return;
        std::string key, val;
        std::string curr;
        bool readingList = false;
        bool inKey = true;
        for (auto i = 0; i < str.size(); i++) {
            auto c = str[i];
    
            switch (c) {
                case '{':
                case '}':
                    break;
                case ':':
                    key = curr;
                    std::cout << key << "\n";
                    curr.clear();
                    inKey = false;
                    break;
                case ',':
                    if (readingList) {
                        curr += c;
                        break;
                    }
                    val = curr;
                    std::cout << val << "\n";
                    json_map[key] = val;
                    curr.clear();
                    inKey = true;
                    break;
                case '[':
                    curr += c;
                    readingList = true;
                    break;
                case ']':
                    curr += c;
                    readingList = false;
                    break;
                default:
                    if (!isspace(c)) curr += c;
            }
        }
    
        if (!key.empty() && !curr.empty()) {
            json_map[key] = curr;  // To handle the last key-value pair
        }
    }

    template<typename... Args>
    JSONBuilder& to_json(Args... args){
        json_map.clear();
        std::cout << "about to make vector\n";
        std::vector<std::string> values{ to_string_helper(std::forward<Args>(args))... };
        std::cout << "made vector\n";
        for (size_t i = 0; i < values.size()-1; i += 2) {
            json_map[values[i]] = values[i+1];
        }
        std::cout << "finished looping\n";
        return *this;
    }

    void load(const std::string& str){
        json_map.clear();
        JSONBuilder new_json(str);
        json_map = new_json.json_map;
    }

    std::string dump(){
        std::string ret = "{\n";
        for (auto [k, v] : json_map){
            std::string key{k}, val{v};
            if (key.empty() or val.empty()) continue;
            if (key[0] != '\"') {
                key = std::string("\"") + key;
                key = key + std::string("\"");
            }
            if (val[0] != '\"') {
                val = std::string("\"") + val;
                val = val + std::string("\"");
            }
            ret += key+": "+val+", ";
        }
        ret.pop_back();
        ret.pop_back();
        return ret += "\n}"; 
    }

    private:
    std::unordered_map<std::string, std::string> json_map;

    template<typename T>
    std::string to_string_helper(const T& value) {
        return std::to_string(value);  // Works for numeric types (int, double, etc.)
    }

    // Specialization for std::string
    std::string to_string_helper(const std::string& value) {
        return value;  // Directly return the string
    }

    // Specialization for const char* (can be passed as a string literal)
    std::string to_string_helper(const char* value) {
        return std::string(value);  // Convert const char* to std::string
    }

};

}  // namespace core

#endif
