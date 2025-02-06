#ifndef LIB_ALGORITHM_H
#define LIB_ALGORITHM_H

namespace core {

// Non-modifying sequence operations

// Batch operations

template<class InputIt, class UnaryFunc>
constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f) {
    for (; first != last; ++first)
        f(*first);

    return f;
}

template<class InputIt, class Size, class UnaryFunc>
constexpr InputIt for_each_n(InputIt first, Size n, UnaryFunc f) {
    for (Size i = 0; i < n; ++first, ++i)
        f(*first);
 
    return first;
}

// Search operations

template<class InputIt, class UnaryPred>
constexpr bool all_of(InputIt first, InputIt last, UnaryPred p) {
    for (; first != last; ++first)
        if (!p(*first)) return false;
    return true;
}

template<class InputIt, class UnaryPred>
constexpr bool any_of(InputIt first, InputIt last, UnaryPred p) {
    for (; first != last; ++first)
        if (p(*first)) return true;
    return false;
}

template<class InputIt, class UnaryPred>
constexpr bool none_of(InputIt first, InputIt last, UnaryPred p) {
    return !::core::any_of(first, last, p);
}

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value) {
    while (first != last && *first != value) ++first;
    return first;
}

template<class InputIt, class UnaryPred>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPred p) {
    for (; first != last; ++first)
        if (p(*first)) return first;
    return last;
}

template<class InputIt, class UnaryPred>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPred q) {
    for (; first != last; ++first)
        if (!q(*first)) return first;
    return last;
}

}  // core

#endif
