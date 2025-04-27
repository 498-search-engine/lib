#ifndef LIB_PAIR_H
#define LIB_PAIR_H


namespace core {
    template<typename T1, typename T2>
    struct Pair{
        T1 first;
        T2 second;

        constexpr bool operator==(const Pair& other) const {
            return first == other.first && second == other.second;
        }

        constexpr bool operator<(const Pair& other) const {
            return (first < other.first) || (!(other.first < first) && second < other.second);
        }
    };

}

#endif