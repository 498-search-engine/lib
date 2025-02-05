#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <array>

using std::vector;

constexpr void foo(int& s, int i) { s+= i; }

TEST(AlgorithmTests, ForEach) {
    const vector<int> v = {1,2,3};

    int sum = 0;
    core::for_each(v.begin(), v.end(), [&](auto i){ sum += i;});
    EXPECT_EQ(sum, 6);

    sum = 0;
    core::for_each(v.begin(), v.begin(), [&](auto i){ sum += i;});
    EXPECT_EQ(sum, 0);

    sum = 0;
    core::for_each(v.begin(), v.begin() + 1, [&](auto i){ sum += i;});
    EXPECT_EQ(sum, 1);
}

TEST(AlgorithmTests, ForEach_constexr) {
   constexpr int sum = []() constexpr {
        constexpr std::array<int, 3> v{1, 2, 3};  // constexpr-compatible array
        int s = 0;
        core::for_each(v.begin(), v.end(), [&s](int i) constexpr { s += i; });
        return s;
    }();
    static_assert(sum == 6);
}

TEST(AlgorithmTests, AnyAllNone) {
    constexpr std::array<int,4> arr{1,2,3,4};
    const auto pnone = [](int i) { return i > 4; };
    EXPECT_TRUE(core::none_of(arr.begin(), arr.end(), pnone));
    EXPECT_FALSE(core::any_of(arr.begin(), arr.end(), pnone));
    EXPECT_FALSE(core::all_of(arr.begin(), arr.end(), pnone));

    const auto pall = [](int i) { return i > 0; };
    EXPECT_FALSE(core::none_of(arr.begin(), arr.end(), pall));
    EXPECT_TRUE(core::any_of(arr.begin(), arr.end(), pall));
    EXPECT_TRUE(core::all_of(arr.begin(), arr.end(), pall));

    const auto pany = [](int i) { return i == 1; };
    EXPECT_FALSE(core::none_of(arr.begin(), arr.end(), pany));
    EXPECT_TRUE(core::any_of(arr.begin(), arr.end(), pany));
    EXPECT_FALSE(core::all_of(arr.begin(), arr.end(), pany));

    const auto ptrue = [](int i) { return true; };
    const vector<int> v{};
    EXPECT_FALSE(core::none_of(arr.begin(), arr.end(), pany));
    EXPECT_FALSE(core::any_of(arr.begin(), arr.end(), pany));
    EXPECT_FALSE(core::all_of(arr.begin(), arr.end(), pany));
}

TEST(AlgorithmTests, AnyAllNone_constexpr) {
    constexpr std::array<int,4> arr{1,2,3,4};
    constexpr auto pnone = [](int i) constexpr { return i > 4; };
    static_assert(core::none_of(arr.begin(), arr.end(), pnone));
    static_assert(!core::any_of(arr.begin(), arr.end(), pnone));
    static_assert(!core::all_of(arr.begin(), arr.end(), pnone));
}
