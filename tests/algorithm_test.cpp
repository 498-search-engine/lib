#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <array>

using std::vector;

constexpr bool iseven(int i) { return i % 2 == 0; }
constexpr bool isodd(int i) { return !iseven(i); }

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
    EXPECT_TRUE(core::none_of(v.begin(), v.end(), ptrue));
    EXPECT_FALSE(core::any_of(v.begin(), v.end(), ptrue));
    EXPECT_TRUE(core::all_of(v.begin(), v.end(), ptrue));
}

TEST(AlgorithmTests, AnyAllNone_constexpr) {
    constexpr std::array<int,4> arr{1,2,3,4};
    constexpr auto pnone = [](int i) constexpr { return i > 4; };
    static_assert(core::none_of(arr.begin(), arr.end(), pnone));
    static_assert(!core::any_of(arr.begin(), arr.end(), pnone));
    static_assert(!core::all_of(arr.begin(), arr.end(), pnone));
}

TEST(AlgorithmTests, Find) {
    vector<int> v{1,3,5,7,8,9};
    for (int i: v) {
        auto it = core::find(v.begin(), v.end(), i);
        EXPECT_TRUE((it != v.end() && *it == i));
    }
    auto it = core::find(v.begin(), v.end(), 0);
    EXPECT_EQ(it, v.end());

    vector<int> empty{};
    it = core::find(empty.begin(), empty.end(), 0);
    EXPECT_EQ(it, empty.end());
}

TEST(AlgorithmTests, FindIf) {
    const vector<int> v{1,3,5,7,8,9};
    auto it = core::find_if(v.begin(), v.end(), iseven);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 8);

    it = core::find_if(v.begin(), v.begin()+3, iseven);
    EXPECT_EQ(it, v.begin()+3);

    vector<int> empty{};
    it = core::find_if(empty.begin(), empty.end(), iseven);
    EXPECT_EQ(it, empty.end());
}

TEST(AlgorithmTests, FindIfNot) {
    const vector<int> v{1,3,5,7,8,9};
    auto it = core::find_if_not(v.begin(), v.end(), isodd);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 8);

    it = core::find_if_not(v.begin(), v.begin()+3, isodd);
    EXPECT_EQ(it, v.begin()+3);

    vector<int> empty{};
    it = core::find_if_not(empty.begin(), empty.end(), iseven);
    EXPECT_EQ(it, empty.end());
}

TEST(AlgorithmTests, FindX_constexpr) {
    constexpr std::array<int, 6> a{1,3,5,7,8,9};
    static_assert(core::find(a.begin(), a.end(), 8) == a.begin()+4);
    static_assert(core::find_if(a.begin(), a.end(), iseven) == a.begin()+4);
    static_assert(core::find_if_not(a.begin(), a.end(), isodd) == a.begin()+4);
}

TEST(AlgorithmTests, Search) {
    const vector<int> text = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const vector<int> pattern = {4, 5, 6};

    auto it = core::search(text.begin(), text.end(), pattern.begin(), pattern.end());
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin()+3);

    auto isequal = [](int a, int b){ return a == b; };
    it = core::search(text.begin(), text.end(), pattern.begin(), pattern.end(), isequal);
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin()+3);

    auto isparity = [](int a, int b) { return (a % 2) == (b % 2);};
    it = core::search(text.begin(), text.end(), pattern.begin(), pattern.end(), isparity);
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin() + 1);

    const vector<int> empty{};
    it = core::search(empty.begin(), empty.end(), pattern.begin(), pattern.end());
    EXPECT_EQ(it, empty.end());
    it = core::search(empty.begin(), empty.end(), pattern.begin(), pattern.end(), isequal);
    EXPECT_EQ(it, empty.end());

    it = core::search(text.begin(), text.end(), empty.begin(), empty.end());
    EXPECT_EQ(it, text.begin());
    it = core::search(text.begin(), text.end(), empty.begin(), empty.end(), isequal);
    EXPECT_EQ(it, text.begin());
}

TEST(AlgorithmTests, Search_constexpr) {
    constexpr std::array<int,9> text{1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr std::array<int,3> pattern{4, 5, 6};

    static_assert(
        core::search(text.begin(), text.end(), pattern.begin(), pattern.end()) == text.begin()+3);

    constexpr auto isequal = [](int a, int b) constexpr { return a == b; };
    static_assert(
        core::search(text.begin(), text.end(), pattern.begin(), pattern.end(), isequal) == text.begin()+3);
}

TEST(AlgorithmTests, FindEnd) {
    const vector<int> text = {1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 5, 6, 10, 11, 12};
    const vector<int> pattern = {4, 5, 6};

    auto it = core::find_end(text.begin(), text.end(), pattern.begin(), pattern.end());
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin()+9);

    auto isequal = [](int a, int b){ return a == b; };
    it = core::find_end(text.begin(), text.end(), pattern.begin(), pattern.end(), isequal);
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin()+9);

    auto isparity = [](int a, int b) { return (a % 2) == (b % 2);};
    it = core::find_end(text.begin(), text.end(), pattern.begin(), pattern.end(), isparity);
    ASSERT_NE(it, text.end());
    EXPECT_EQ(it, text.begin() + 12);

    const vector<int> empty{};
    it = core::find_end(empty.begin(), empty.end(), pattern.begin(), pattern.end());
    EXPECT_EQ(it, empty.end());
    it = core::find_end(empty.begin(), empty.end(), pattern.begin(), pattern.end(), isequal);
    EXPECT_EQ(it, empty.end());

    it = core::find_end(text.begin(), text.end(), empty.begin(), empty.end());
    EXPECT_EQ(it, text.end());
    it = core::find_end(text.begin(), text.end(), empty.begin(), empty.end(), isequal);
    EXPECT_EQ(it, text.end());
}

TEST(AlgorithmTests, FindEnd_constexpr) {
    constexpr std::array<int,15> text{1, 2, 3, 4, 5, 6, 7, 8, 9, 4, 5, 6, 10, 11, 12};
    constexpr std::array<int,3> pattern{4, 5, 6};

    static_assert(
        core::find_end(text.begin(), text.end(), pattern.begin(), pattern.end()) == text.begin()+9);

    constexpr auto isequal = [](int a, int b) constexpr { return a == b; };
    static_assert(
        core::find_end(text.begin(), text.end(), pattern.begin(), pattern.end(), isequal) == text.begin()+9);
}

TEST(AlgorithmTests, FindFirstOf) {
    auto isequal = [](int a, int b){ return a == b; };
    // Case 1: Empty haystack
    std::vector<int> empty_haystack;
    std::vector<int> needles = {1, 2, 3};
    EXPECT_EQ(core::find_first_of(empty_haystack.begin(), empty_haystack.end(),
                            needles.begin(), needles.end()), empty_haystack.end());
    EXPECT_EQ(core::find_first_of(empty_haystack.begin(), empty_haystack.end(),
                            needles.begin(), needles.end(), isequal), empty_haystack.end());

    // Case 2: Empty needle
    std::vector<int> haystack = {4, 5, 6, 7, 8};
    std::vector<int> empty_needles;
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            empty_needles.begin(), empty_needles.end()), haystack.end());
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            empty_needles.begin(), empty_needles.end(), isequal), haystack.end());

    // Case 3: No matches
    std::vector<int> no_match_needles = {1, 2, 3};
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            no_match_needles.begin(), no_match_needles.end()), haystack.end());
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            no_match_needles.begin(), no_match_needles.end(), isequal), haystack.end());

    // Case 4: First element is a match
    std::vector<int> first_match_needles = {4, 9, 10};
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            first_match_needles.begin(), first_match_needles.end()), haystack.begin());
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            first_match_needles.begin(), first_match_needles.end(), isequal), haystack.begin());

    // Case 5: Last element is a match
    std::vector<int> last_match_needles = {8, 9, 10};
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            last_match_needles.begin(), last_match_needles.end()), haystack.begin() + 4);
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            last_match_needles.begin(), last_match_needles.end(), isequal), haystack.begin() + 4);

    // Case 6: Middle element is a match
    std::vector<int> middle_match_needles = {6, 10, 11};
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            middle_match_needles.begin(), middle_match_needles.end()), haystack.begin() + 2);
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            middle_match_needles.begin(), middle_match_needles.end(), isequal), haystack.begin() + 2);

    // Case 7: Multiple matches, return first occurrence
    std::vector<int> multiple_match_needles = {7, 8, 6};
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            multiple_match_needles.begin(), multiple_match_needles.end()), haystack.begin() + 2);
    EXPECT_EQ(core::find_first_of(haystack.begin(), haystack.end(),
                            multiple_match_needles.begin(), multiple_match_needles.end(), isequal), haystack.begin() + 2);

    // Case 8: Custom binary predicate
    std::string str_haystack = "HelloWorld";
    std::string str_needles = "ow";
    auto it = core::find_first_of(str_haystack.begin(), str_haystack.end(),
                            str_needles.begin(), str_needles.end(),
                            [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    EXPECT_EQ(it, str_haystack.begin() + 4);
}
