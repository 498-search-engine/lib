#ifndef LIB_ALGORITHM_H
#define LIB_ALGORITHM_H

#include <iterator>
#include <type_traits>
#include <functional>
#include <initializer_list>

namespace core {

// Non-modifying sequence operations

// Batch operations

template<class InputIt, class UnaryFunc>
constexpr UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f);

template<class InputIt, class Size, class UnaryFunc>
constexpr InputIt for_each_n(InputIt first, Size n, UnaryFunc f);

// Search operations

template<class InputIt, class UnaryPred>
constexpr bool all_of(InputIt first, InputIt last, UnaryPred p);

template<class InputIt, class UnaryPred>
constexpr bool any_of(InputIt first, InputIt last, UnaryPred p);

template<class InputIt, class UnaryPred>
constexpr bool none_of(InputIt first, InputIt last, UnaryPred p);

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value);

template<class InputIt, class UnaryPred>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPred p);

template<class InputIt, class UnaryPred>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPred q);

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last);

template< class ForwardIt1, class ForwardIt2, class BinaryPred >
constexpr ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
                  ForwardIt2 s_first, ForwardIt2 s_last,
                  BinaryPred p);

template<class ForwardIt, class Searcher>
ForwardIt search(ForwardIt first, ForwardIt last,
                 const Searcher& searcher);

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last);

template< class ForwardIt1, class ForwardIt2, class BinaryPred >
constexpr ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last,
                    BinaryPred p);

template<class InputIt, class ForwardIt>
constexpr InputIt find_first_of(InputIt first, InputIt last,
                      ForwardIt s_first, ForwardIt s_last);

template<class InputIt, class ForwardIt, class BinaryPred>
constexpr InputIt find_first_of(InputIt first, InputIt last,
                      ForwardIt s_first, ForwardIt s_last,
                      BinaryPred p);

template<class ForwardIt>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last);

template<class ForwardIt, class BinaryPred>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPred p);

template<class InputIt, class T>
constexpr typename std::iterator_traits<InputIt>::difference_type
    count(InputIt first, InputIt last, const T& value);

template<class InputIt, class UnaryPred>
constexpr typename std::iterator_traits<InputIt>::difference_type
    count_if(InputIt first, InputIt last, UnaryPred p);

// TODO: std::mismatch() (requires std::pair)

template<class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2);

template<class InputIt1, class InputIt2, class BinaryPred>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, BinaryPred p);

template<class InputIt1, class InputIt2>
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, InputIt2 last2);

template< class InputIt1, class InputIt2, class BinaryPred >
constexpr bool equal(InputIt1 first1, InputIt1 last1,
                     InputIt2 first2, InputIt2 last2, BinaryPred p);

template<class ForwardIt, class Size, class T>
constexpr ForwardIt search_n(ForwardIt first, ForwardIt last,
                             Size count, const T& value);

// Modifying sequence operations

// Copy operations

template<class InputIt, class OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first);

template<class InputIt, class OutputIt, class UnaryPred>
constexpr OutputIt copy_if(InputIt first, InputIt last,
                           OutputIt d_first, UnaryPred pred);

template<class InputIt, class Size, class OutputIt>
constexpr OutputIt copy_n(InputIt first, Size count, OutputIt result);

template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last);

template<class InputIt, class OutputIt>
constexpr OutputIt move(InputIt first, InputIt last, OutputIt d_first);

template<class BidirIt1, class BidirIt2>
constexpr BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last);

template<class ForwardIt, class T>
constexpr void fill(ForwardIt first, ForwardIt last, const T& value);

template<class OutputIt, class Size, class T>
constexpr OutputIt fill_n(OutputIt first, Size count, const T& value);

template<class InputIt, class OutputIt, class UnaryOp>
constexpr OutputIt transform(InputIt first1, InputIt last1,
                             OutputIt d_first, UnaryOp unary_op);

template<class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
constexpr OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             OutputIt d_first, BinaryOp binary_op);

template<class ForwardIt, class Generator>
constexpr void generate(ForwardIt first, ForwardIt last, Generator g);

template<class OutputIt, class Size, class Generator>
constexpr OutputIt generate_n(OutputIt first, Size count, Generator g);

template<class ForwardIt, class T>
constexpr ForwardIt remove(ForwardIt first, ForwardIt last, const T& value);

template<class ForwardIt, class UnaryPred>
constexpr ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPred p);

template<class InputIt, class OutputIt, class T>
constexpr OutputIt remove_copy(InputIt first, InputIt last,
                               OutputIt d_first, const T& value);

template<class InputIt, class OutputIt, class UnaryPred>
constexpr OutputIt remove_copy_if(InputIt first, InputIt last,
                                  OutputIt d_first, UnaryPred p);

template<class ForwardIt, class T>
constexpr void replace(ForwardIt first, ForwardIt last,
                       const T& old_value, const T& new_value);

template<class ForwardIt, class UnaryPred, class T>
constexpr void replace_if(ForwardIt first, ForwardIt last,
                          UnaryPred p, const T& new_value);

template< class InputIt, class OutputIt, class T >
constexpr OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first,
                                const T& old_value, const T& new_value);

template<class InputIt, class OutputIt, class UnaryPred, class T>
constexpr OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first,
                                   UnaryPred p, const T& new_value);

template<class T>
constexpr void swap(T& a, T& b) 
noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>);

template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b);

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1,
                                 ForwardIt2 first2);

template<class BidirIt>
constexpr void reverse(BidirIt first, BidirIt last);

template<class BidirIt, class OutputIt>
constexpr OutputIt reverse_copy(BidirIt first, BidirIt last,
                                OutputIt d_first);

template<class ForwardIt>
constexpr ForwardIt rotate(ForwardIt first, ForwardIt middle, ForwardIt last);

template<class ForwardIt>
constexpr ForwardIt shift_left(ForwardIt first, ForwardIt last,
                               typename std::iterator_traits<ForwardIt>::difference_type n);

template<class ForwardIt>
constexpr ForwardIt shift_right(ForwardIt first, ForwardIt last,
                               typename std::iterator_traits<ForwardIt>::difference_type n);

// Sorted Ranges

// Binary Search

template< class ForwardIt, class T, class Compare >
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last,
                                const T& value, Compare comp);

template<class ForwardIt, class T>
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last,
                                const T& value);

template<class ForwardIt, class T, class Compare>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last,
                                const T& value, Compare comp);

template<class ForwardIt, class T>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last,
                                const T& value);

template<class ForwardIt, class T, class Compare>
constexpr bool binary_search(ForwardIt first, ForwardIt last,
                             const T& value, Compare comp);

template<class ForwardIt, class T>
constexpr bool binary_search(ForwardIt first, ForwardIt last,
                             const T& value);

// Miscellaneous comparison operations

// Max
template<class T>
constexpr const T& max(const T& a, const T& b);

template<class T, class Compare>
constexpr const T& max(const T& a, const T& b, Compare comp);

template<class T>
constexpr T max(std::initializer_list<T> ilist);

template<class T, class Compare>
constexpr T max(std::initializer_list<T> ilist, Compare comp);

// Max Element
template<class ForwardIt>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last);

template<class ForwardIt, class Compare>
constexpr ForwardIt max_element(ForwardIt first, ForwardIt last,
                                Compare comp);

// Min
template<class T>
constexpr const T& min(const T& a, const T& b);

template<class T, class Compare>
constexpr const T& min(const T& a, const T& b, Compare comp);

template<class T>
constexpr T min(std::initializer_list<T> ilist);

template<class T, class Compare>
constexpr T min(std::initializer_list<T> ilist, Compare comp);

// Min Element
template<class ForwardIt>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last);

template<class ForwardIt, class Compare>
constexpr ForwardIt min_element(ForwardIt first, ForwardIt last,
                                Compare comp);

// Clamp
template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi);

template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi,
                         Compare comp);

// Clamp Range
template<class ForwardIt, class T>
constexpr void clamp_range(ForwardIt first, ForwardIt last, const T& lo, const T& hi);

template<class ForwardIt, class T, class Compare>
constexpr void clamp_range(ForwardIt first, ForwardIt last,
                           const T& lo, const T& hi, Compare comp);

}  // core

#include "core/internal/algorithm_impl.h"

#endif
