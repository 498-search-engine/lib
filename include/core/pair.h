#ifndef LIB_PAIR_H
#define LIB_PAIR_H
#include <type_traits>
#include <utility> // for std::move and std::forward

namespace core {

template <typename First, typename Second>
struct Pair {
    First first;
    Second second;

    // Default constructor
    constexpr Pair() = default;

    // Move constructor
    constexpr Pair(First&& f, Second&& s)
        : first(std::move(f)), second(std::move(s)) {}

    // Copy constructor
    template <typename F = First, typename S = Second,
              typename = std::enable_if_t<std::is_copy_constructible_v<F> && std::is_copy_constructible_v<S>>>
    constexpr Pair(const F& f, const S& s)
        : first(f), second(s) {}

    // Perfect forwarding constructor
    template <typename F, typename S,
              typename = std::enable_if_t<!std::is_same_v<Pair, std::decay_t<F>>>>
    constexpr Pair(F&& f, S&& s)
        : first(std::forward<F>(f)), second(std::forward<S>(s)) {}

    // Copy constructor and move constructor defaulted if available
    constexpr Pair(const Pair&) = default;
    constexpr Pair(Pair&&) noexcept = default;

    constexpr Pair& operator=(const Pair&) = default;
    constexpr Pair& operator=(Pair&&) noexcept = default;

    // Comparison operators
    constexpr bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }

    constexpr bool operator!=(const Pair& other) const {
        return !(*this == other);
    }

    constexpr bool operator<(const Pair& other) const {
        return (first < other.first) || (!(other.first < first) && second < other.second);
    }

    constexpr bool operator<=(const Pair& other) const {
        return !(other < *this);
    }

    constexpr bool operator>(const Pair& other) const {
        return other < *this;
    }

    constexpr bool operator>=(const Pair& other) const {
        return !(*this < other);
    }
};

template<typename First, typename Second>
constexpr Pair<std::decay_t<First>, std::decay_t<Second>> make_pair(First&& f, Second&& s) {
    return Pair<std::decay_t<First>, std::decay_t<Second>>(std::forward<First>(f), std::forward<Second>(s));
}


} // namespace core

#endif