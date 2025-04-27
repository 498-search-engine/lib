#ifndef LIB_STRING_VIEW_H
#define LIB_STRING_VIEW_H

#include "core/algorithm.h"
#include "core/internal/string_fn.h"
#include "core/iterator.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string_view>

namespace core {

class StringView {
public:
    using const_iterator = PointerIteratorWrapper<const char>;

    static constexpr size_t npos = -1;

    static constexpr size_t Npos = -1;

    constexpr StringView(std::string_view sv) noexcept
    : data_(sv.data()), size_(sv.size()) {}

    constexpr StringView() noexcept : data_(nullptr), size_(0) {}
    constexpr StringView(const char* s, size_t n) : data_(s), size_(n) {}
    constexpr StringView(const char* s) : data_(s), size_(internal::StrLen(s)) {}
    constexpr StringView(std::nullptr_t) = delete;

    constexpr const_iterator begin() const { return const_iterator{data_}; }
    constexpr const_iterator end() const { return const_iterator{data_ + size_}; }
    constexpr const_iterator cbegin() const { return const_iterator{data_}; }
    constexpr const_iterator cend() const { return const_iterator{data_ + size_}; }

    constexpr const char& operator[](size_t i) const { return data_[i]; }
    constexpr char Front() const { return data_[0]; }
    constexpr char Back() const { return data_[size_ - 1]; }

    constexpr const char* Data() const { return data_; }
    constexpr size_t Size() const { return size_; }
    constexpr size_t Length() const { return size_; }
    constexpr bool Empty() const { return size_ == 0; }

    constexpr void RemovePrefix(size_t n) {
        assert(size_ >= n);
        data_ += n;
        size_ -= n;
    }

    constexpr void RemoveSuffix(size_t n) {
        assert(size_ >= n);
        size_ -= n;
    }
    constexpr StringView Substr(size_t pos, size_t count = Npos) {
        pos = std::min(pos, size_);
        size_t remaining = size_ - pos;
        if (count == Npos || count > remaining) {
            count = remaining;
        }
        return StringView{data_ + pos, count};
    }


    constexpr int Compare(StringView v) const { return internal::Compare(Data(), Size(), v.Data(), v.Size()); }
    constexpr int Compare(const char* s) const { return internal::Compare(Data(), Size(), s, internal::StrLen(s)); }

    constexpr bool StartsWith(StringView v) const noexcept {
        return internal::StartsWith(Data(), Size(), v.Data(), v.Size());
    }
    constexpr bool StartsWith(char c) const noexcept { return size_ > 0 && Front() == c; }
    constexpr bool StartsWith(const char* s) const {
        return internal::StartsWith(Data(), Size(), s, internal::StrLen(s));
    }

    constexpr bool EndsWith(StringView v) const noexcept {
        return internal::EndsWith(Data(), Size(), v.Data(), v.Size());
    }
    constexpr bool EndsWith(char c) const noexcept { return size_ > 0 && Back() == c; }
    constexpr bool EndsWith(const char* s) const { return internal::EndsWith(Data(), Size(), s, internal::StrLen(s)); }

    constexpr size_t Find(StringView v, size_t pos = 0) const {
        if (pos > Size()) {
            throw std::out_of_range("pos is out of range");
        }
        return internal::Find<Npos>(Data(), Size(), v.Data(), v.Size(), pos);
    }
    constexpr size_t Find(char c, size_t pos = 0) const {
        if (pos > Size()) {
            throw std::out_of_range("pos is out of range");
        }
        return internal::FindChar<Npos>(Data(), Size(), c, pos);
    }
    constexpr size_t Find(const char* s, size_t pos = 0) const noexcept {
        return internal::Find<Npos>(Data(), Size(), s, internal::StrLen(s), pos);
    }

    constexpr char front() const { return Front(); }
constexpr char back() const { return Back(); }

constexpr const char* data() const { return Data(); }
constexpr size_t size() const { return Size(); }
constexpr size_t length() const { return Length(); }
constexpr bool empty() const { return Empty(); }

constexpr void remove_prefix(size_t n) { RemovePrefix(n); }
constexpr void remove_suffix(size_t n) { RemoveSuffix(n); }
constexpr StringView substr(size_t pos, size_t count = Npos) { return Substr(pos, count); }

constexpr int compare(StringView v) const { return Compare(v); }
constexpr int compare(const char* s) const { return Compare(s); }

constexpr bool starts_with(StringView v) const noexcept { return StartsWith(v); }
constexpr bool starts_with(char c) const noexcept { return StartsWith(c); }
constexpr bool starts_with(const char* s) const { return StartsWith(s); }

constexpr bool ends_with(StringView v) const noexcept { return EndsWith(v); }
constexpr bool ends_with(char c) const noexcept { return EndsWith(c); }
constexpr bool ends_with(const char* s) const { return EndsWith(s); }

constexpr size_t find(StringView v, size_t pos = 0) const { return Find(v, pos); }
constexpr size_t find(char c, size_t pos = 0) const { return Find(c, pos); }
constexpr size_t find(const char* s, size_t pos = 0) const noexcept { return Find(s, pos); }

private:
    const char* data_;
    size_t size_;
};

constexpr bool operator==(StringView lhs, StringView rhs) noexcept {
    return lhs.Size() == rhs.Size() && core::equal(lhs.begin(), lhs.end(), rhs.begin());
}

constexpr bool operator<(StringView lhs, StringView rhs) noexcept {
    return lhs.Compare(rhs) < 0;
}

constexpr bool operator<=(StringView lhs, StringView rhs) noexcept {
    return lhs.Compare(rhs) <= 0;
}

constexpr bool operator>(StringView lhs, StringView rhs) noexcept {
    return lhs.Compare(rhs) > 0;
}

constexpr bool operator>=(StringView lhs, StringView rhs) noexcept {
    return lhs.Compare(rhs) >= 0;
}

namespace string_view_literals {

constexpr StringView operator""_sv(const char* s, size_t n) noexcept {
    return StringView{s, n};
}

}  // namespace string_view_literals

}  // namespace core

namespace std {

template<>
struct hash<core::StringView> {
    std::size_t operator()(core::StringView s) const {
        constexpr std::hash<std::string_view> Hasher;
        return Hasher(std::string_view{s.Data(), s.Size()});
    }
};

}  // namespace std

#endif
