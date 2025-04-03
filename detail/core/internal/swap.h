/**
 * @file swap.h
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

#ifndef LIB_SWAP_IMPL_H
#define LIB_SWAP_IMPL_H

#include <iterator>
#include <type_traits>
#include <functional>

namespace core {

    template<class T>
    constexpr void swap(T& a, T& b) 
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
    requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>)
    {
        T t(std::move(a));
        a = std::move(b);
        b = std::move(t);
    }

}  // core

#endif
