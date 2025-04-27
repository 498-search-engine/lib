#ifndef LIB_PAIR_H
#define LIB_PAIR_H


namespace core {
    template<typename First, typename Second>
    struct Pair{
        First first;
        Second second;

        constexpr Pair() = default;

        // Copy constructor
        constexpr Pair(const First& f, const Second& s) : first(f), second(s) {}
    
        // Move constructor
        constexpr Pair(First&& f, Second&& s) noexcept(std::is_nothrow_move_constructible_v<First> && std::is_nothrow_move_constructible_v<Second>)
            : first(std::move(f)), second(std::move(s)) {}
    
        // Constructor from std::pair
        template <typename T1, typename T2>
        constexpr Pair(const std::pair<T1, T2>& p) : first(p.first), second(p.second) {}
    
        // Move from std::pair
        template <typename T1, typename T2>
        constexpr Pair(std::pair<T1, T2>&& p) : first(std::move(p.first)), second(std::move(p.second)) {}
    
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