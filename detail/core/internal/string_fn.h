#ifndef LIB_STRING_FN_H
#define LIB_STRING_FN_H

#include "core/algorithm.h"
#include <cstddef>

namespace core::internal {

constexpr size_t StrLen(const char* s) {
    size_t n = 0;
    while (*s != '\0') {
        ++s;
        ++n;
    }
    return n;
}

constexpr int Compare(const char* s1, size_t n1, const char* s2, size_t n2) noexcept {
    size_t minLen = (n1 < n2) ? n1 : n2;
    for (size_t i = 0; i < minLen; ++i) {
        if (s1[i] < s2[i]) {
            return -1;
        }
        if (s1[i] > s2[i]) {
            return 1;
        }
    }
    // Common prefix at this point
    if (n1 == n2) {
        return 0;
    }
    return (n1 < n2) ? -1 : 1;
}

constexpr bool StartsWith(const char* s, size_t n, const char* prefix, size_t prefixN) noexcept {
    if (prefixN > n) {
        return false;
    }
    for (size_t i = 0; i < prefixN; ++i) {
        if (s[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

constexpr bool EndsWith(const char* s, size_t n, const char* suffix, size_t suffixN) noexcept {
    if (suffixN > n) {
        return false;
    }

    s = s + n - suffixN;
    for (size_t i = 0; i < suffixN; ++i) {
        if (s[i] != suffix[i]) {
            return false;
        }
    }
    return true;
}

template<size_t Npos>
constexpr size_t Find(const char* s, size_t n, const char* target, size_t targetN, size_t pos) noexcept {
    const auto* it = core::search(s + pos, s + n, target, target + targetN);
    if (it == s + n) {
        return Npos;
    } else {
        return it - s;
    }
}

template<size_t Npos>
constexpr size_t FindChar(const char* s, size_t n, char c, size_t pos) noexcept {
    const auto* it = core::find(s + pos, s + n, c);
    if (it == s + n) {
        return Npos;
    } else {
        return it - s;
    }
}

}  // namespace core::internal


#endif
