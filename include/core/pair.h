#ifndef LIB_PAIR_H
#define LIB_PAIR_H


namespace core {
    template<typename First, typename Second>
    struct Pair{
        First first;
        Second second;

        // Default constructor
    constexpr Pair() = default;

    // Two-argument constructor
    constexpr Pair(const First& f, const Second& s) : first(f), second(s) {}

    // Move constructor
    constexpr Pair(First&& f, Second&& s) : first(std::move(f)), second(std::move(s)) {}

    // Allow implicit conversion from std::pair
    template <typename T1, typename T2>
    constexpr Pair(const std::pair<T1, T2>& p) : first(p.first), second(p.second) {}

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

    constexpr bool operator>(const Pair& other) const {
        return other < *this;
    }

    constexpr bool operator<=(const Pair& other) const {
        return !(other < *this);
    }

    constexpr bool operator>=(const Pair& other) const {
        return !(*this < other);
    }
    };

}

#endif