#include "core/optional.h"

#include <type_traits>
#include <gtest/gtest.h>


TEST(OptionalTests, constexpr_stuff) {
    constexpr Optional<int> empty1{};
    constexpr Optional<int> empty2(nullopt);
    constexpr Optional<int> empty3 = {};
    constexpr Optional<int> empty4 = {nullopt};
    constexpr Optional<int> empty5 = nullopt;

    static_assert(!empty1);
    static_assert(!empty2);
    static_assert(!empty3);
    static_assert(!empty4);
    static_assert(!empty5);

    constexpr Optional<int> val1{1};
    constexpr Optional<int> val2(1);
    constexpr Optional<int> val3 = 1;
    constexpr Optional<int> val4 = {1};

    constexpr int num = 2;
    constexpr Optional<int> mov1{std::move(num)};
    constexpr Optional<int> mov2(std::move(num));
    constexpr Optional<int> mov3 = {std::move(num)};
    constexpr Optional<int> mov4 = std::move(num);
}

template<typename T> constexpr bool is_trivial = std::is_trivially_destructible<T>::value;
TEST(OptionalTests, TypeTraits) {
    static_assert(is_trivial<int>);
    static_assert(is_trivial<Optional<int>>);
}
