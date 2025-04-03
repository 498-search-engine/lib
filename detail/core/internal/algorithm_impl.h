/**
 * @file algorithm_impl.h
 * @author Christopher Davis chrsdavis
 * @brief Implementation details of <core/algorithm.h>, do not include
 * @version 1.0
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LIB_ALGORITHM_H
#error "Do not include this file directly. Include <core/algorithm.h> instead."
#endif

#ifndef LIB_ALGORITHM_IMPL_H
#define LIB_ALGORITHM_IMPL_H

#include <iterator>
#include <type_traits>
#include <functional>

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

template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (last != first) {
        *(--d_last) = std::move(*(--last));
    }
    return d_last;
}

template<class ForwardIt, class T>
constexpr void fill(ForwardIt first, ForwardIt last, const T& value) {
    for (; first != last; ++first)
        *first = value;
}

template<class OutputIt, class Size, class T>
constexpr OutputIt fill_n(OutputIt first, Size count, const T& value) {
    for (Size i = 0; i < count; ++i, ++first)
        *first = value;
    return first;
}

template<class InputIt, class OutputIt, class UnaryOp>
constexpr OutputIt transform(InputIt first1, InputIt last1,
                             OutputIt d_first, UnaryOp unary_op)
{
    for (; first1 != last1; ++first1, ++d_first)
        *d_first = unary_op(*first1);
    return d_first;
}

template<class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
constexpr OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             OutputIt d_first, BinaryOp binary_op)
{
    for (; first1 != last1; ++first1, ++first2, ++d_first)
        *d_first = binary_op(*first1, *first2);
    return d_first;
}

template<class ForwardIt, class Generator>
constexpr void generate(ForwardIt first, ForwardIt last, Generator g) {
    for (; first != last; ++first)
        *first = g();
}

template<class OutputIt, class Size, class Generator>
constexpr OutputIt generate_n(OutputIt first, Size count, Generator g) {
    for (Size i = 0; i < count; ++i, ++first)
        *first = g();
    return first;
}

template<class ForwardIt, class T>
constexpr ForwardIt remove(ForwardIt first, ForwardIt last, const T& value) {
    first = core::find(first, last, value);
    for (ForwardIt it = first; it != last; ++it) {
        if (!(*it == value)) {
            *first = std::move(*it);
            ++first;
        }
    }
    return first;
}

template<class ForwardIt, class UnaryPred>
constexpr ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPred p) {
    first = core::find_if(first, last, p);
    for (ForwardIt it = first; it != last; ++it) {
        if (!p(*it)) {
            *first = std::move(*it);
            ++first;
        }
    }
    return first;
}

template<class InputIt, class OutputIt, class T>
constexpr OutputIt remove_copy(InputIt first, InputIt last,
                               OutputIt d_first, const T& value)
{
    for (; first != last; ++first) {
        if (!(*first == value))
            *d_first++ = *first;
    }
    return d_first;
}

template<class InputIt, class OutputIt, class UnaryPred>
constexpr OutputIt remove_copy_if(InputIt first, InputIt last,
                                  OutputIt d_first, UnaryPred p)
{
    for (; first != last; ++first)
        if (!p(*first))
            *d_first++ = *first;
    return d_first;
}

template<class ForwardIt, class T>
constexpr void replace(ForwardIt first, ForwardIt last,
                       const T& old_value, const T& new_value)
{
    for (; first != last; ++first) {
        if (*first == old_value)
            *first = new_value;
    }
}

template<class ForwardIt, class UnaryPred, class T>
constexpr void replace_if(ForwardIt first, ForwardIt last,
                          UnaryPred p, const T& new_value)
{
    for (; first != last; ++first) {
        if (p(*first)) *first = new_value;
    }
}

template< class InputIt, class OutputIt, class T >
constexpr OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first,
                                const T& old_value, const T& new_value)
{
    for (; first != last; ++first, ++d_first)
        *d_first = (*first == old_value) ? new_value : *first;
    return d_first;
}

template<class InputIt, class OutputIt, class UnaryPred, class T>
constexpr OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first,
                                   UnaryPred p, const T& new_value)
{
    for (; first != last; ++first, ++d_first)
        *d_first = p(*first) ? new_value : *first;
    return d_first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) {
    core::swap(*a, *b);
}

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1,
                                 ForwardIt2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
        core::iter_swap(first1, first2);
    return first2;
}

template<class BidirIt>
constexpr void reverse(BidirIt first, BidirIt last) {
    // optimize random access it into for loop for compiler
    if constexpr (std::random_access_iterator<BidirIt>) {
        if (first == last) return;
        --last; // decrement from end() to inside the range
        for (; first < last; ++first, --last) // normal for-loop
            core::iter_swap(first, last);
    } else {
        while (first != last && first != --last) {
            core::iter_swap(first, last);
            ++first;
        }
    }
}

template<class BidirIt, class OutputIt>
constexpr OutputIt reverse_copy(BidirIt first, BidirIt last,
                                OutputIt d_first)
{
    for (; first != last; ++d_first)
        *d_first = *(--last); // must decrement last before assignment
    return d_first;
}

template<class ForwardIt>
constexpr ForwardIt rotate(ForwardIt first, ForwardIt middle, ForwardIt last) {
    if (first == middle) return last;
    if (middle == last) return first;
    // Optimize for different iterator categories
    if constexpr (std::random_access_iterator<ForwardIt>) {
        core::reverse(first, middle);
        core::reverse(middle, last);
        core::reverse(first, last);
        return first + (last - middle);
    } else if constexpr (std::bidirectional_iterator<ForwardIt>) {
        core::reverse(first, middle);
        core::reverse(middle, last);
        while(first != middle && last != middle) {
            --last;
            core::iter_swap(first, last);
            ++first;
        }
        core::reverse(first, last);
        return (first != middle) ? first : last;
    } else { // Forwards only iterator
        auto cur = first, next = middle;
        do {
            core::iter_swap(cur, next);
            ++cur;
            ++next;
            if (cur == middle) middle = next;
        } while (next != last);
        first = cur; // update new first pos
        while (middle != last) {
            next = middle;
            do {
                core::iter_swap(cur, next);
                ++cur;
                ++next;
                if (cur == middle) middle = next;
            } while (next != last);
        }
    }
    return first;
}

template<class ForwardIt>
constexpr ForwardIt shift_left(ForwardIt first, ForwardIt last,
                               typename std::iterator_traits<ForwardIt>::difference_type n)
{
    if (n <= 0) return last;
    if constexpr (std::random_access_iterator<ForwardIt>) {
        if (n >= last - first) return first;
        return core::move(first + n, last, first);
    } else {
        auto new_first = first;  // find new first
        for (auto i = 0; i < n; ++i) {
            if (new_first == last) return first; // n >= dist(first,last)
            ++new_first;
        }
        return core::move(new_first, last, first);
    }
}

template<class ForwardIt>
constexpr ForwardIt shift_right(ForwardIt first, ForwardIt last,
                               typename std::iterator_traits<ForwardIt>::difference_type n)
{
    if (n <= 0) return first;
    if constexpr (std::random_access_iterator<ForwardIt>) {
        if (n >= last - first) return last;
        return core::move_backward(first, last - n, last);
    } else if constexpr (std::bidirectional_iterator<ForwardIt>) {
        auto new_last = last;
        for (auto i = 0; i < n; ++i) {
            if (new_last == first) return last;
            --new_last;
        }
        return core::move_backward(first, new_last, last);
    } else {
        // TODO: requires vector for external buffering
        static_assert(false, "Not Implemented");
        return last;
    }
}

// Sorted Ranges

// Binary Search

template< class ForwardIt, class T, class Compare >
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last,
                                const T& value, Compare comp)
{
    if constexpr (std::random_access_iterator<ForwardIt>) {
        while (first < last) {
            auto mid = first + (last - first) / 2;
            if (comp(*mid, value)) {
                first = mid + 1;
            } else {
                last = mid;
            }
        }
        return first;
    } else {
        return core::find_if(first, last, [&](const auto& elem){ return !comp(elem, value); });
    }
}

template<class ForwardIt, class T>
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last,
                                const T& value)
{
    return core::lower_bound(first, last, value, std::less{});
}

template<class ForwardIt, class T, class Compare>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last,
                                const T& value, Compare comp)
{
    if constexpr (std::random_access_iterator<ForwardIt>) {
        while (first < last) {
            auto mid = first + (last - first) / 2;
            if (comp(value, *mid)) {
                last = mid;
            } else {
                first = mid + 1;
            }
        }
        return first;
    } else {
        return core::find_if(first, last, [&](const auto& elem){ return comp(value, elem); });
    }
}

template<class ForwardIt, class T>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last,
                                const T& value)
{
    return core::upper_bound(first, last, value, std::less{});
}

template<class ForwardIt, class T, class Compare>
constexpr bool binary_search(ForwardIt first, ForwardIt last,
                             const T& value, Compare comp)
{
    first = core::lower_bound(first, last, value, comp);
    return (first != last && !comp(value, *first));
}

template<class ForwardIt, class T>
constexpr bool binary_search(ForwardIt first, ForwardIt last,
                             const T& value)
{
    return core::binary_search(first, last, value, std::less{});
}

// Miscellaneous comparison operations

template<class T>
constexpr const T& max(const T& a, const T& b) {
    return core::max(a, b, std::less{});
}

template<class T, class Compare>
constexpr const T& max(const T& a, const T& b, Compare comp) {
    return comp(a,b) ? b : a;
}

template<class T>
constexpr T max(std::initializer_list<T> ilist) {
    return *core::max_element(ilist.begin(), ilist.end());
}

template<class T, class Compare>
constexpr T max(std::initializer_list<T> ilist, Compare comp) {
    return *core::max_element(ilist.begin(), ilist.end(), comp);
}

template<class ForwardIt>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last) {
    return core::max_element(first, last, std::less{});
}

template<class ForwardIt, class Compare>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last,
                                Compare comp)
{
    if (first == last) return last;
    ForwardIt it = first;
    for (; first != last; ++first) {
        if (comp(*it, *first)) 
            it = first;
    }
    return it;
}

template<class T>
constexpr const T& min(const T& a, const T& b) {
    return core::min(a, b, std::less{});
}

template<class T, class Compare>
constexpr const T& min(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? a : b;
}

template<class T>
constexpr T min(std::initializer_list<T> ilist) {
    return *core::min_element(ilist.begin(), ilist.end());
}

template<class T, class Compare>
constexpr T min(std::initializer_list<T> ilist, Compare comp) {
    return *core::min_element(ilist.begin(), ilist.end(), comp);
}

template<class ForwardIt>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last) {
    return core::min_element(first, last, std::less{});
}

template<class ForwardIt, class Compare>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last,
                                Compare comp)
{
    if (first == last) return last;
    ForwardIt it = first;
    for (; first != last; ++first) {
        if (comp(*first, *it)) 
            it = first;
    }
    return it;
}

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return core::clamp(v, lo, hi, std::less{});
}

template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi,
                         Compare comp)
{
    return comp(v, lo) ? lo : (comp(hi, v) ? hi : v);
}

template<class ForwardIt, class T>
constexpr void clamp_range(ForwardIt first, ForwardIt last, const T& lo, const T& hi) {
    for (; first != last; ++first)
        *first = clamp(*first, lo, hi);
}

template<class ForwardIt, class T, class Compare>
constexpr void clamp_range(ForwardIt first, ForwardIt last,
                           const T& lo, const T& hi, Compare comp)
{
    for (; first != last; ++first)
        *first = clamp(*first, lo, hi, comp);
}

}  // core

#endif
