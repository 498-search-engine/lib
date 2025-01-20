#ifndef LIB_OPTIONAL_H
#define LIB_OPTIONAL_H

#include <utility>
#include <type_traits>
#include <stdexcept>

struct nullopt_t {};
inline static constexpr nullopt_t nullopt;

class BadOptionalAccess final : public std::exception {
public:
    const char* what() const noexcept override {
        return "Bad optional access: Attempted to access value of an empty Optional";
    }
};

template<typename T>
class Optional {
public:
    constexpr Optional() noexcept : Optional(nullopt) {}
    constexpr Optional(nullopt_t) noexcept : sentinel_{0}, has_value_{false} {}

    constexpr Optional(const Optional& opt) requires std::is_trivially_copy_constructible_v<T> = default;
    constexpr Optional(const Optional& opt) 
        requires (std::is_copy_constructible_v<T> && !std::is_trivially_copy_constructible_v<T>)
        : has_value_{opt.has_value_} {
        // TODO: handle copying val_ from opt
    }
    constexpr Optional(Optional&& opt) requires std::is_trivially_move_constructible_v<T> = default;
    constexpr Optional(Optional&& opt)
        noexcept(std::is_nothrow_move_assignable_v<T>)
        requires (std::is_move_constructible_v<T> && !std::is_trivially_move_constructible_v)
        : has_value_{opt.has_value_} {
        // TODO: handle moving val_
    }

    // TODO: conversion constructors

    constexpr ~Optional() requires std::is_trivially_destructible_v<T> = default;
    constexpr ~Optional() { if (has_value_) val_.~T(); }

public:
    // Observers
    constexpr const T* operator->() const noexcept { return &val_; }
    constexpr T* operator->() noexcept { return &val_; }

    constexpr const T& operator*() const& noexcept { return val_; }
    constexpr T& operator*() & noexcept { return val_; }
    constexpr const T&& operator*() const&& noexcept { return std::move(val_); }
    constexpr T&& operator*() && noexcept { return std::move(val_); }

    constexpr explicit operator bool() const noexcept { return has_value_; }

    [[nodiscard]]
    constexpr bool HasValue() const noexcept { return has_value_; }

    [[nodiscard]]
    constexpr T& Value() & { return has_value_ ? val_ : throw BadOptionalAccess; }
    [[nodiscard]]
    constexpr const T& Value() const & { return has_value_ ? val_ : throw BadOptionalAccess; }
    [[nodiscard]]
    constexpr T&& value() && { return has_value_ ? std::move(val_) : throw BadOptionalAccess; }
    [[nodiscard]]
    constexpr const T&& value() const && { return has_value_ ? std::move(val_) : throw BadOptionalAccess; }

    // TODO: add type constraints here https://en.cppreference.com/w/cpp/utility/optional/value_or
    template<typename U >
    constexpr T ValueOr( U&& default_value ) const& { 
        return has_value_ ? val_ : static_cast<T>(std::forward<U>(default_value));
    }
    template<typename U >
    constexpr T ValueOr( U&& default_value ) && {
        return has_value_ ? std::move(val_) : static_cast<T>(std::forward<U>(default_value));
    }

    // Monadic Operations (C++23)
    // TODO: maybe add these?

    // Iterators (C++26)
    // FIXME: add custom iterator type later
    constexpr T* begin() noexcept { return has_value_ ? &val_ : nullptr; }
    constexpr const T* begin() const noexcept { return has_value_ ? &val_ : nullptr; }
    constexpr T* end() noexcept { return has_value_ ? &val_ + 1 : nullptr; }
    constexpr const T* end() const noexcept { return has_value_ ? &val_ + 1 : nullptr; }

    // Modifiers

private:
    union { char sentinel_; T val_; };
    bool has_value_;
};

#endif
