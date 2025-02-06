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

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last)
{
    if (s_first == s_last) return first;
    while (first != last) {
        ForwardIt1 it1 = first;
        ForwardIt2 it2 = s_first;
        while (it1 != last && it2 != s_last && *it1 == *it2) {
            ++it1, ++it2;
        }
        if (it2 == s_last) { // found sequence
            return first;
        } else if (it1 == last) { // no match, end of searching
            return last;
        } else {
            ++first; // search from next element
        }
    }
    return last; // didn't find anything
}

template< class ForwardIt1, class ForwardIt2, class BinaryPred >
constexpr ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last,
                  BinaryPred p)
{
    if (s_first == s_last) return first;
    while (first != last) {
        ForwardIt1 it1 = first;
        ForwardIt2 it2 = s_first;
        while (it1 != last && it2 != s_last && p(*it1, *it2)) {
            ++it1, ++it2;
        }
        if (it2 == s_last) { // found sequence
            return first;
        } else if (it1 == last) { // no match, end of searching
            return last;
        } else {
            ++first; // search from next element
        }
    }
    return last; // didn't find anything
}

template<class ForwardIt, class Searcher>
ForwardIt search(ForwardIt first, ForwardIt last,
                 const Searcher& searcher)
{
    return searcher(first, last).first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last)
{
    if (s_first == s_last) return last;

    ForwardIt1 pos = last;
    while (first != last) {
        const auto it = ::core::search(first, last, s_first, s_last);
        if (it == last) break; // last occurence
        pos = it;
        first = it;
        ++first;
    }
    return pos;
}

template< class ForwardIt1, class ForwardIt2, class BinaryPred >
constexpr ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last,
                    BinaryPred p)
{
    if (s_first == s_last) return last;

    ForwardIt1 pos = last;
    while (first != last) {
        const auto it = ::core::search(first, last, s_first, s_last, p);
        if (it == last) break; // last occurence
        pos = it;
        first = it;
        ++first;
    }
    return pos;
}

}  // core

#endif
