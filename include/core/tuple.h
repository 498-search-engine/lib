#ifndef LIB_TUPLE_H
#define LIB_TUPLE_H

#include <utility>
#include <cstddef>
#include <type_traits>

namespace core {

template<typename... T>
struct Tuple;

template<>
struct Tuple<> {};

template<typename Head, typename... Tail>
struct Tuple<Head, Tail...> {
    Head head;
    Tuple<Tail...> tail;

    Tuple() = default;

    
    template<typename HH, typename... TT, typename = std::enable_if_t<!std::is_same_v<std::decay_t<HH>, Tuple>>>
    Tuple(HH&& h, TT&&... t)
        : head(std::forward<HH>(h)),
          tail(std::forward<TT>(t)...) {}

    // Copy constructor
    Tuple(const Tuple& other)
        : head(other.head),
          tail(other.tail) {}

    // Move constructor
    Tuple(Tuple&& other) noexcept
        : head(std::move(other.head)),
          tail(std::move(other.tail)) {}

    // Copy assignment
    Tuple& operator=(const Tuple& other) {
        head = other.head;
        tail = other.tail;
        return *this;
    }

    // Move assignment
    Tuple& operator=(Tuple&& other) noexcept {
        head = std::move(other.head);
        tail = std::move(other.tail);
        return *this;
    }
};


template<std::size_t Index, typename TupleType>
struct tuple_element;

// Recursive case
template<std::size_t Index, typename Head, typename... Tail>
struct tuple_element<Index, Tuple<Head, Tail...>> {
    using type = typename tuple_element<Index - 1, Tuple<Tail...>>::type;

    static type& get(Tuple<Head, Tail...>& tuple) {
        return tuple_element<Index - 1, Tuple<Tail...>>::get(tuple.tail);
    }
    static const type& get(const Tuple<Head, Tail...>& tuple) {
        return tuple_element<Index - 1, Tuple<Tail...>>::get(tuple.tail);
    }
};

// Base case
template<typename Head, typename... Tail>
struct tuple_element<0, Tuple<Head, Tail...>> {
    using type = Head;

    static type& get(Tuple<Head, Tail...>& tuple) {
        return tuple.head;
    }
    static const type& get(const Tuple<Head, Tail...>& tuple) {
        return tuple.head;
    }
};


template<std::size_t Index, typename... Types>
auto& get(Tuple<Types...>& t) {
    return tuple_element<Index, Tuple<Types...>>::get(t);
}

template<std::size_t Index, typename... Types>
const auto& get(const Tuple<Types...>& t) {
    return tuple_element<Index, Tuple<Types...>>::get(t);
}

} // namespace core

#endif
