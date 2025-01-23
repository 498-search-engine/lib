#include "core/optional.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <utility>


TEST(OptionalTests, Emplace) {
    constexpr int i = 1;
    Optional<int> o1;
    EXPECT_EQ(o1, nullopt);
    EXPECT_FALSE(o1);
    o1.Emplace(i);
    ASSERT_TRUE(o1);
    EXPECT_EQ(o1, 1);

    o1.Emplace(o1.Value());
    ASSERT_TRUE(o1);
    EXPECT_EQ(o1, i);
}

TEST(OptionalTests, EmplaceInitList) {
    Optional<std::vector<int>> o1;
    EXPECT_FALSE(o1);
    o1.Emplace({1, 2, 3, 4});
    ASSERT_TRUE(o1.HasValue());
    EXPECT_EQ(o1->size(), 4);
    const std::vector<int> v{1,2,3,4};
    EXPECT_EQ(o1.Value(), v);
}

TEST(OptionalTests, Swap) {
    Optional<int> o1{1};
    Optional<int> o2{2};
    o1.Swap(o2);
    ASSERT_TRUE(o1 && o2);
    EXPECT_EQ(o2, 1);
    EXPECT_EQ(o1, 2);
}

TEST(OptionalTests, SwapNullopt) {
    Optional<int> o1{1};
    Optional<int> o2{};
    o1.Swap(o2);
    ASSERT_TRUE(o2);
    EXPECT_FALSE(o1);
    EXPECT_EQ(o2, 1);

    Optional<int> o3{}, o4{}, o5{5};
    o3.Swap(o4);
    EXPECT_FALSE(o3 || o4);

    o3.Swap(o5);
    EXPECT_FALSE(o5);
    ASSERT_TRUE(o3);
    EXPECT_EQ(o3, 5);
}

TEST(OptionalTests, BadOptionalAccessError) {
    Optional<int> opt;
    EXPECT_THROW({int i = opt.Value();}, BadOptionalAccess);
    EXPECT_THROW({const auto& i = opt.Value();}, BadOptionalAccess);
    EXPECT_THROW(++opt.Value(), BadOptionalAccess);
}

TEST(OptionalTests, Comparisons) {
    constexpr int i = 1, j = 2;
    Optional<int> empty{};
    EXPECT_TRUE(empty == nullopt);
    EXPECT_TRUE(empty >= nullopt);
    EXPECT_TRUE(empty <= nullopt);

    Optional<int> o1{i};
    EXPECT_TRUE(o1 != nullopt);
    EXPECT_FALSE(o1 == nullopt);
    EXPECT_TRUE(o1 > nullopt);

    EXPECT_TRUE(o1 > empty);
    EXPECT_TRUE(o1 >= empty);
    EXPECT_TRUE(empty != o1);

    EXPECT_TRUE(o1 == i);
    EXPECT_TRUE(i == o1);
    EXPECT_TRUE(o1 < j);
    EXPECT_TRUE(j > o1);
    EXPECT_TRUE(o1 <= j);
    EXPECT_TRUE(j >= o1);
    EXPECT_EQ(o1 <=> i, std::strong_ordering::equal);
    EXPECT_EQ(o1 <=> j, std::strong_ordering::less);

    Optional<int> o2{j};
    EXPECT_TRUE(o1 < o2);
    EXPECT_TRUE(o1 <= o2);
    EXPECT_TRUE(o1 != o2);
    EXPECT_EQ(o1 <=> o2, std::strong_ordering::less);

    Optional<int> o3{i};
    EXPECT_TRUE(o1 == o3);
    EXPECT_EQ(o1 <=> o3, std::strong_ordering::equal);
}

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
    static_assert(empty1.ValueOr(3) == 3);

    constexpr Optional<int> val1{1};
    constexpr Optional<int> val2(1);
    constexpr Optional<int> val3 = 1;
    constexpr Optional<int> val4 = {1};

    static_assert(val1 == 1);
    static_assert(val2 == 1);
    static_assert(val3 == 1);
    static_assert(val4 == 1);

    static_assert(val1.Value() == 1);

    constexpr int num = 2;
    constexpr Optional<int> mov1{std::move(num)};
    constexpr Optional<int> mov2(std::move(num));
    constexpr Optional<int> mov3 = {std::move(num)};
    constexpr Optional<int> mov4 = std::move(num);

    static_assert(mov1 == 2);
    static_assert(mov2 == 2);
    static_assert(mov3 == 2);
    static_assert(mov4 == 2); 

    static_assert(val1 < mov1);
    static_assert(val1 <= mov1);
    static_assert(mov1 > val1);
    static_assert(mov1 >= val2);
    static_assert(val1 != mov1);
    static_assert((val1 <=> mov1) == std::strong_ordering::less);
    static_assert((val1 <=> 1) == std::strong_ordering::equal);
}

template<typename T> constexpr bool is_trivial = std::is_trivially_destructible<T>::value;
TEST(OptionalTests, TypeTraits) {
    static_assert(is_trivial<int>);
    static_assert(is_trivial<Optional<int>>);
}
