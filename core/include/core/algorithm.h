#ifndef LIB_ALGORITHM_H
#define LIB_ALGORITHM_H

#include <iterator>

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
    while (first != last && !(*first == value)) ++first;
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

template<class InputIt, class ForwardIt>
constexpr InputIt find_first_of(InputIt first, InputIt last,
                      ForwardIt s_first, ForwardIt s_last)
{
    if (s_first == s_last) return last;

    for (; first != last; ++first) {
        for (ForwardIt it = s_first; it != s_last; ++it) {
            if (*it == *first) return first;
        }
    }
    return last;
}

template<class InputIt, class ForwardIt, class BinaryPred>
constexpr InputIt find_first_of(InputIt first, InputIt last,
                      ForwardIt s_first, ForwardIt s_last,
                      BinaryPred p)
{
    if (s_first == s_last) return last;

    for (; first != last; ++first) {
        for (ForwardIt it = s_first; it != s_last; ++it) {
            if (p(*it, *first)) return first;
        }
    }
    return last;
}

template<class ForwardIt>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last) {
    if (first == last) return last;
    ForwardIt it = first;
    ++it;
    for (; it != last; ++it, ++first) {
        if (*it == *first) return first;
    }
    return last;
}

template<class ForwardIt, class BinaryPred>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPred p) {
    if (first == last) return last;
    ForwardIt it = first;
    ++it;
    for (; it != last; ++it, ++first) {
        if (p(*it, *first)) return first;
    }
    return last;
}

template<class InputIt, class T>
constexpr typename std::iterator_traits<InputIt>::difference_type
    count(InputIt first, InputIt last, const T& value)
{
    typename std::iterator_traits<InputIt>::difference_type count = 0;
    for (; first != last; ++first) {
        if (*first == value) ++count;
    }
    return count;
}

template<class InputIt, class UnaryPred>
constexpr typename std::iterator_traits<InputIt>::difference_type
    count_if(InputIt first, InputIt last, UnaryPred p)
{
    typename std::iterator_traits<InputIt>::difference_type count = 0;
    for (; first != last; ++first) {
        if (p(*first)) ++count;
    }
    return count;
}

// TODO: std::mismatch() (requires std::pair)

template<class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2)) return false;
    }
    return true;
}

template<class InputIt1, class InputIt2, class BinaryPred>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, BinaryPred p)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!p(*first1, *first2)) return false;
    }
    return true;
}

template<class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, InputIt2 last2)
{
    constexpr bool is_random_access =
        std::random_access_iterator<InputIt1> && std::random_access_iterator<InputIt2>;
    if constexpr (is_random_access) {
        if (last1 - first1 != last2 - first2)
            return false;
    }

    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (!(*first1 == *first2)) return false;
    }

    if constexpr (is_random_access) {
        return true; // already checked they match length
    } else {
        return first1 == last1 && first2 == last2;
    }
}

template< class InputIt1, class InputIt2, class BinaryPred >
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, InputIt2 last2, BinaryPred p)
{
    constexpr bool is_random_access =
        std::random_access_iterator<InputIt1> && std::random_access_iterator<InputIt2>;
    if constexpr (is_random_access) {
        if (last1 - first1 != last2 - first2)
            return false;
    }

    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (!p(*first1, *first2)) return false;
    }

    if constexpr (is_random_access) {
        return true; // already checked they match length
    } else {
        return first1 == last1 && first2 == last2;
    }
}

template<class ForwardIt, class Size, class T>
constexpr ForwardIt search_n(ForwardIt first, ForwardIt last,
                             Size count, const T& value)
{
    if (count <= 0) return first;

    first = ::core::find(first, last, value);
    while (first != last) {
        Size cur_count{0};
        const auto candidate = first;
        while (first != last && *first == value && cur_count < count) {
            ++cur_count;
            ++first;
        }
        if (cur_count == count) return candidate;
        first = ::core::find(first, last, value);
    }
    return last;
}

// Modifying sequence operations

// Copy operations

template<class InputIt, class OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first)
        *d_first = *first;
    return d_first;
}

template<class InputIt, class OutputIt, class UnaryPred>
constexpr OutputIt copy_if(InputIt first, InputIt last,
                           OutputIt d_first, UnaryPred pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            *d_first = *first;
            ++d_first;
        }
    }
    return d_first;
}

template<class InputIt, class Size, class OutputIt>
constexpr OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
    while (count > 0) {
        *result = *first;
        ++result, ++first;
        --count;
    }
    return result;
}

template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (last != first) {
        *(--d_last) = *(--last);
    }
    return d_last;
}

template<class InputIt, class OutputIt>
constexpr OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
    for (; first != last; ++first, ++d_first) {
        *d_first = std::move(*first);
    }
    return d_first;
}

}  // core

#endif
