#include "core/optional.h"

#include <type_traits>
#include <gtest/gtest.h>


TEST(OptionalTests, constexpr_stuff) {
    // ...
}

template<typename T> constexpr bool is_trivial = std::is_trivially_destructible<T>::value;
TEST(OptionalTests, TypeTraits) {
    static_assert(is_trivial<int>);
    static_assert(is_trivial<Optional<int>>);
}
