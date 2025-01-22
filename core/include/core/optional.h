#ifndef LIB_OPTIONAL_H
#define LIB_OPTIONAL_H

#include <utility>
#include <type_traits>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <algorithm>
#include <concepts>
#include <compare>


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
    // Helper type traits
    template<class... Args>
    static constexpr bool is_constructible_from_any_v = (std::is_constructible_v<T, Args> || ...);

    template<class... Args>
    static constexpr bool is_convertible_from_any_v = (std::is_convertible_v<Args, T> || ...);

    template<class... Args>
    static constexpr bool is_assignable_from_any_v = (std::is_assignable_v<Args, T> || ...);

    template<typename U, typename V /*= U& or U&&*/>
    static constexpr bool satisfies_conv_ctor = std::is_constructible_v<T, V> &&
        !is_constructible_from_any_v<Optional<U>&, const Optional<U>&, Optional<U>&&, const Optional<U>&&> &&
        !is_convertible_from_any_v<Optional<U>&, const Optional<U>&, Optional<U>&&, const Optional<U>&&>;

public:
    constexpr Optional() noexcept : Optional(nullopt) {}
    constexpr Optional(nullopt_t) noexcept : sentinel_{0}, has_value_{false} {}

    constexpr Optional(const Optional& opt) requires std::is_trivially_copy_constructible_v<T> = default;
    constexpr Optional(const Optional& opt) 
        requires (std::is_copy_constructible_v<T> && !std::is_trivially_copy_constructible_v<T>)
        : has_value_{opt.has_value_}
    {
        DirectInitFromOpt(opt);
    }

    constexpr Optional(Optional&& opt) requires std::is_trivially_move_constructible_v<T> = default;
    constexpr Optional(Optional&& opt)
        noexcept(std::is_nothrow_move_assignable_v<T>)
        requires (std::is_move_constructible_v<T> && !std::is_trivially_move_constructible_v)
        : has_value_{opt.has_value_}
    {
        DirectInitFromOpt(std::forward(opt));
    }

    template<typename U>
    constexpr explicit(!std::is_convertible_v<const U&, T>) Optional(const Optional<U>& opt)
        requires satisfies_conv_ctor<U, const U&>
        : has_value_{opt.has_value_}
    {
        DirectInitFromOpt(opt);
    }

    template<typename U>
    constexpr explicit(!std::is_convertible_v<const U&, T>) Optional(Optional<U>&& opt)
        requires satisfies_conv_ctor<U, U&&>
        : has_value_{opt.has_value_}
    {
        DirectInitFromOpt(std::forward(opt));
    }

    // TODO: in place constructors

    template <typename U = T>
    constexpr explicit(!std::is_convertible_v<U&&, T>) Optional(U&& val)
        requires (std::is_constructible_v<T, U&&> &&
                  !std::is_same_v<std::remove_cvref_t<U>, Optional>)
        : has_value_{true}
    {
        DirectInitVal(val);
    }

    constexpr ~Optional() requires std::is_trivially_destructible_v<T> = default;
    constexpr ~Optional() { if (has_value_) val_.~T(); }

    static constexpr bool is_trivially_copyable_v = std::is_trivially_copy_constructible_v<T> && 
        std::is_trivially_copy_assignable_v<T> && std::is_trivially_destructible_v<T>;
    static constexpr bool is_trivially_moveable_v = std::is_trivially_move_constructible_v<T> &&
        std::is_trivially_move_assignable_v<T> && std::is_trivially_destructible_v<T>;

    // Assignment
    constexpr Optional& operator=(nullopt_t) noexcept { Reset(); }
    constexpr Optional& operator=(const Optional& opt) requires is_trivially_copyable_v = default;
    constexpr Optional& operator=(const Optional& opt)
        requires (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && !is_trivially_copyable_v)
    {
        AssignFromOpt(opt);
        return val_;
    }
    constexpr Optional& operator=(Optional&& opt)
        noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>)
        requires is_trivially_moveable_v = default;
    constexpr Optional& operator=(Optional&& opt)
        noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>)
        requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && !is_trivially_moveable_v)
    {
        AssignFromOpt(std::forward(opt));
        return val_;
    }

    template<typename U = T>
    constexpr Optional& operator=(U&& value) requires
        (std::is_constructible_v<T, U> && std::is_assignable_v<T&, U> &&
        !std::is_same_v<std::remove_cvref_t<U>, Optional> &&
        (!std::is_scalar_v<T> || !std::is_same_v<std::decay_t<U>, T>))
    {
        if (has_value_) { // val_ exists, so assign
            val_ = std::forward(value);
        } else { // must direct init it
            DirectInitVal(std::forward(value));
        }
        return val_;
    }

    template<typename U, typename UU>
    static constexpr bool satisfies_assign_reqs = satisfies_conv_ctor<U, UU> && std::is_assignable_v<T&, UU>
        !is_assignable_from_any_v<Optional<U>&, const Optional<U>&, Optional<U>&&, const Optional<U>&&>;

    template<typename U>
    constexpr Optional& operator=(const Optional<U>& opt) requires satisfies_assign_reqs<U, const U&> {
        AssignFromOpt(opt);
        return val_;
    }

    template<typename U>
    constexpr Optional& operator=(Optional<U>&& opt) requires satisfies_assign_reqs<U, U> {
        AssignFromOpt(std::forward(opt));
        return val_;
    }

    // Iterators (C++26)
    // FIXME: add custom iterator type later
    constexpr T* begin() noexcept { return has_value_ ? &val_ : nullptr; }
    constexpr const T* begin() const noexcept { return has_value_ ? &val_ : nullptr; }
    constexpr T* end() noexcept { return has_value_ ? &val_ + 1 : nullptr; }
    constexpr const T* end() const noexcept { return has_value_ ? &val_ + 1 : nullptr; }

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
    constexpr T ValueOr(U&& default_value) const& { 
        return has_value_ ? val_ : static_cast<T>(std::forward<U>(default_value));
    }
    template<typename U >
    constexpr T ValueOr(U&& default_value) && {
        return has_value_ ? std::move(val_) : static_cast<T>(std::forward<U>(default_value));
    }

    // Monadic Operations (C++23)
    // TODO: maybe add these?

    // Modifiers
    constexpr void swap(Optional& other)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>)
        requires std::is_move_constructible_v<T>
    {
        if (this->has_value_ && other.has_value_) {
            // TODO: use our own swap later
            std::swap(this->val_, other.val_);
        } else if (this->has_value_) {
            other.DirectInitVal(std::move(this->val_));
            this->ForceReset();
        } else if (other.has_value_) {
            this->DirectInitVal(std::move(other.val_));
            other.ForceReset();
        }
    }

    constexpr void Reset() noexcept {
        if (has_value_) ForceReset();
    }

    // TODO: check Args can construct T?
    template<class... Args>
    constexpr T& Emplace(Args&&... args) {
        Reset();
        DirectInitVal(std::forward(args));
    }

    // TODO: check Args can construct T?
    template<typename U, class... Args>
    constexpr T& Emplace(std::initializer_list<U> ilist, Args&&... args)
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>
    {
        Reset();
        DirectInitVal(ilist, std::forward(args));
    }

private:
    // assumes has_value_ is true
    constexpr void ForceReset() {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            val_.~T();
        }
        has_value_ = false;
    }

    // TODO: check T is constructible by Args?
    template<class... Args>
    constexpr void DirectInitVal(Args&&... args) {
        std::construct_at(&val_, std::forward<Args>(args)...);
        has_value_ = true;
    }

    // TODO: constrain Opt
    template<typename Opt>
    constexpr void DirectInitFromOpt(Opt&& opt) {
        if (opt.has_value_) DirectInitVal(*std::forward(opt));
        else sentinel_ = 0; // Required for constexpr
    }

    // TODO: constrain Opt
    template<typename Opt>
    constexpr void AssignFromOpt(Opt&& opt) {
        if (this->has_value_ && opt.has_value_) {
            val_ = *std::forward(opt);
        } else if (this->has_value_) {
            ForceReset();
        } else if (opt.has_value_) {
            DirectInitVal(std::forward(*opt));
        }
    }

private:
    union { char sentinel_; T val_; };
    bool has_value_;
};

// Comparison Operators
// compare with another optional
template<class T, class U>
constexpr bool operator==(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs == *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs == *rhs);
    else return (lhs.HasValue() == rhs.HasValue());
}

template<class T, class U>
constexpr bool operator!=(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs != *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs == *rhs);
    else return (lhs.HasValue() != rhs.HasValue());
}

template<class T, class U>
constexpr bool operator<(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs < *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs < *rhs);
    else return !lhs.HasValue();
}

template<class T, class U>
constexpr bool operator<=(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs <= *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs <= *rhs);
    else return (lhs.HasValue() <= rhs.HasValue());
}

template<class T, class U>
constexpr bool operator>(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs > *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs > *rhs);
    else return lhs.HasValue();
}

template<class T, class U>
constexpr bool operator<=(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs >= *rhs} -> std::convertible_to<bool>;}
{
    if (lhs && rhs) return (*lhs >= *rhs);
    else return (lhs.HasValue() >= rhs.HasValue());
}

template<class T, std::three_way_comparable_with<T> U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const Optional<T>& lhs, const Optional<U>& rhs)
    requires requires {{*lhs <=> *rhs} -> std::convertible_to<std::compare_three_way_result_t<T, U>>;}
{
    return lhs && rhs ? *lhs <=> *rhs : lhs.HasValue() <=> rhs.HasValue();
}

// compare with nullopt
template<class T>
constexpr bool operator==(const Optional<T>& opt, nullopt_t) noexcept {
    return !opt.HasValue();
}

template<class T>
constexpr std::strong_ordering operator<=>(const Optional<T>& opt, nullopt_t) noexcept {
    return (opt.HasValue() <=> false);
}

// compare with value
template<class T, class U>
constexpr bool operator==(const Optional<T>& opt, const U& value)
    requires requires {{*opt == value} -> std::convertible_to<bool>;}
{
    return opt ? *opt == value : false;
}
template<class T, class U>
constexpr bool operator==(const Optional<T>& opt, const U& value)
    requires requires {{value == *opt} -> std::convertible_to<bool>;}
{
    return opt ? value == *opt : false;
}

template<class T, class U>
constexpr bool operator!=(const Optional<T>& opt, const U& value)
    requires requires {{*opt != value} -> std::convertible_to<bool>;}
{
    return opt ? *opt != value : false;
}
template<class T, class U>
constexpr bool operator!=(const Optional<T>& opt, const U& value)
    requires requires {{value != *opt} -> std::convertible_to<bool>;}
{
    return opt ? value != *opt : false;
}

template<class T, class U>
constexpr bool operator<(const Optional<T>& opt, const U& value)
    requires requires {{*opt < value} -> std::convertible_to<bool>;}
{
    return opt ? *opt < value : false;
}
template<class T, class U>
constexpr bool operator<(const Optional<T>& opt, const U& value)
    requires requires {{value < *opt} -> std::convertible_to<bool>;}
{
    return opt ? value < *opt : false;
}

template<class T, class U>
constexpr bool operator<=(const Optional<T>& opt, const U& value)
    requires requires {{*opt <= value} -> std::convertible_to<bool>;}
{
    return opt ? *opt == value : false;
}
template<class T, class U>
constexpr bool operator<=(const Optional<T>& opt, const U& value)
    requires requires {{value <= *opt} -> std::convertible_to<bool>;}
{
    return opt ? value == *opt : false;
}

template<class T, class U>
constexpr bool operator>(const Optional<T>& opt, const U& value)
    requires requires {{*opt > value} -> std::convertible_to<bool>;}
{
    return opt ? *opt > value : false;
}
template<class T, class U>
constexpr bool operator>(const Optional<T>& opt, const U& value)
    requires requires {{value > *opt} -> std::convertible_to<bool>;}
{
    return opt ? value > *opt : false;
}

template<class T, class U>
constexpr bool operator>=(const Optional<T>& opt, const U& value)
    requires requires {{*opt >= value} -> std::convertible_to<bool>;}
{
    return opt ? *opt >= value : false;
}
template<class T, class U>
constexpr bool operator>=(const Optional<T>& opt, const U& value)
    requires requires {{value >= *opt} -> std::convertible_to<bool>;}
{
    return opt ? value >= *opt : false;
}

template<class T, std::three_way_comparable_with<T> U>
constexpr std::compare_three_way_result_t<T, U> operator<=>( const Optional<T>& opt, const U& value )
    requires requires {{*opt <=> value} -> std::convertible_to<std::compare_three_way_result_t<T, U>>;}
{
    return opt ? ? *opt <=> value : std::strong_ordering::less;
}

#endif
