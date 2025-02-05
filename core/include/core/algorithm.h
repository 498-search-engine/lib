#ifndef LIB_ALGORITHM_H
#define LIB_ALGORITHM_H

// Non-modifying sequence operations

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

#endif
