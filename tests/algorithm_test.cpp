#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <array>
#include <list>

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

TEST(AlgorithmTests, FindFirstOf_constexpr) {
    // Ensure it all work in constexpr context
    constexpr std::array<int,3> needles = {6, 10, 11};
    constexpr std::array<int,5> haystack = {4, 5, 6, 7, 8};
    auto isequal = [](int a, int b){ return a == b; };
    static_assert(core::find_first_of(haystack.begin(), haystack.end(),
                            needles.begin(), needles.end()) == haystack.begin() + 2);
    static_assert(core::find_first_of(haystack.begin(), haystack.end(),
                            needles.begin(), needles.end(), isequal) == haystack.begin() + 2);
}

TEST(AlgorithmTests, AdjacentFind) {
    // Case 1: Empty range
    std::vector<int> empty;
    EXPECT_EQ(core::adjacent_find(empty.begin(), empty.end()), empty.end());

    // Case 2: No adjacent duplicates
    std::vector<int> no_adjacent_duplicates = {1, 2, 3, 4, 5};
    EXPECT_EQ(core::adjacent_find(no_adjacent_duplicates.begin(), no_adjacent_duplicates.end()),
              no_adjacent_duplicates.end());

    // Case 3: Adjacent duplicates at the beginning
    std::vector<int> begin_duplicates = {2, 2, 3, 4, 5};
    EXPECT_EQ(core::adjacent_find(begin_duplicates.begin(), begin_duplicates.end()), begin_duplicates.begin());

    // Case 4: Adjacent duplicates in the middle
    std::vector<int> middle_duplicates = {1, 2, 3, 3, 4, 5};
    EXPECT_EQ(core::adjacent_find(middle_duplicates.begin(), middle_duplicates.end()), middle_duplicates.begin() + 2);

    // Case 5: Adjacent duplicates at the end
    std::vector<int> end_duplicates = {1, 2, 3, 4, 5, 5};
    EXPECT_EQ(core::adjacent_find(end_duplicates.begin(), end_duplicates.end()), end_duplicates.begin() + 4);

    // Case 6: All elements are the same
    std::vector<int> all_same = {7, 7, 7, 7, 7};
    EXPECT_EQ(core::adjacent_find(all_same.begin(), all_same.end()), all_same.begin());

    // Case 7: Custom predicate - consecutive even numbers
    std::vector<int> custom_pred = {1, 3, 5, 6, 8, 9};
    auto pred = [](int a, int b) { return (a % 2 == 0) && (b % 2 == 0); };
    EXPECT_EQ(core::adjacent_find(custom_pred.begin(), custom_pred.end(), pred), custom_pred.begin() + 3);

    // Case 8: Custom predicate - no match
    std::vector<int> no_match = {1, 3, 5, 7, 9};
    EXPECT_EQ(core::adjacent_find(no_match.begin(), no_match.end(), pred), no_match.end());

    // Case 9: Single element range
    std::vector<int> single_element = {42};
    EXPECT_EQ(core::adjacent_find(single_element.begin(), single_element.end()), single_element.end());
}

TEST(AlgorithmTests, AdjacentFind_constexpr) {
    // Ensure constexpr works ok
    constexpr std::array<int,6> arr{1, 2, 3, 3, 4, 5};
    static_assert(core::adjacent_find(arr.begin(), arr.end()) == arr.begin() + 2);

    constexpr std::array<int,6> arr2{1, 3, 5, 6, 8, 9};
    auto pred = [](int a, int b) { return (a % 2 == 0) && (b % 2 == 0); };
    static_assert(
        core::adjacent_find(arr2.begin(), arr2.end(), pred) == arr2.begin() + 3);
}

TEST(AlgorithmTests, Count) {
    // Case 1: Empty range
    std::vector<int> empty;
    EXPECT_EQ(core::count(empty.begin(), empty.end(), 42), 0);

    // Case 2: No occurrences of the target element
    std::vector<int> no_match = {1, 2, 3, 4, 5};
    EXPECT_EQ(core::count(no_match.begin(), no_match.end(), 10), 0);

    // Case 3: Single occurrence of the target element
    std::vector<int> single_occurrence = {1, 2, 3, 4, 5};
    EXPECT_EQ(core::count(single_occurrence.begin(), single_occurrence.end(), 3), 1);

    // Case 4: Multiple occurrences of the target element
    std::vector<int> multiple_occurrences = {1, 3, 3, 3, 4, 5};
    EXPECT_EQ(core::count(multiple_occurrences.begin(), multiple_occurrences.end(), 3), 3);

    // Case 5: All elements match the target
    std::vector<int> all_match = {7, 7, 7, 7, 7};
    EXPECT_EQ(core::count(all_match.begin(), all_match.end(), 7), 5);

    // Case 6: Target element appears at the beginning and end
    std::vector<int> boundary_match = {2, 3, 4, 2};
    EXPECT_EQ(core::count(boundary_match.begin(), boundary_match.end(), 2), 2);

    // Case 7: String test case
    std::string str_test = "hello world";
    EXPECT_EQ(core::count(str_test.begin(), str_test.end(), 'o'), 2);
}

TEST(AlgorithmTests, CountIf) {
    // Case 1: Empty range
    std::vector<int> empty;
    EXPECT_EQ(core::count_if(empty.begin(), empty.end(), [](int) { return true; }), 0);

    // Case 2: No elements satisfy the predicate
    std::vector<int> no_match = {1, 2, 3, 4, 5};
    EXPECT_EQ(core::count_if(no_match.begin(), no_match.end(), [](int x) { return x > 10; }), 0);

    // Case 3: Single element satisfies the predicate
    std::vector<int> single_match = {1, 2, 3, 4, 5};
    EXPECT_EQ(core::count_if(single_match.begin(), single_match.end(), [](int x) { return x == 3; }), 1);

    // Case 4: Multiple elements satisfy the predicate
    std::vector<int> multiple_matches = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(core::count_if(multiple_matches.begin(), multiple_matches.end(), [](int x) { return x % 2 == 0; }), 3);

    // Case 5: All elements satisfy the predicate
    std::vector<int> all_match = {7, 7, 7, 7, 7};
    EXPECT_EQ(core::count_if(all_match.begin(), all_match.end(), [](int x) { return x == 7; }), 5);

    // Case 6: Some elements satisfy the predicate (odd numbers)
    std::vector<int> mixed_values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(core::count_if(mixed_values.begin(), mixed_values.end(), [](int x) { return x % 2 != 0; }), 5);

    // Case 7: Predicate checks for positive numbers
    std::vector<int> positive_negative = {-5, -3, 0, 2, 4, 6};
    EXPECT_EQ(core::count_if(positive_negative.begin(), positive_negative.end(), [](int x) { return x > 0; }), 3);

    // Case 8: String test case (count vowels)
    std::string str_test = "hello world";
    EXPECT_EQ(core::count_if(str_test.begin(), str_test.end(), [](char c) { return c == 'o'; }), 2);
}

TEST(AlgorithmTests, CountX_constexpr) {
    constexpr std::array<int,6> arr{-5, -3, 0, 2, 4, 6};
    static_assert(
        core::count_if(arr.begin(), arr.end(), [](int x) { return x > 0; }) == 3);
    static_assert(core::count(arr.begin(), arr.end(), 0) == 1);
}

// TODO: core::mismatch tests

TEST(AlgorithmTests, Equal_unsafe) {
    // Tests for unsafe core::Equal() overload

    // Case 1: Empty ranges (should be equal)
    std::vector<int> empty1, empty2;
    EXPECT_TRUE(core::equal(empty1.begin(), empty1.end(), empty2.begin()));

    // Case 2: Equal ranges
    std::vector<int> same1 = {1, 2, 3, 4, 5};
    std::vector<int> same2 = {1, 2, 3, 4, 5};
    EXPECT_TRUE(core::equal(same1.begin(), same1.end(), same2.begin()));

    // Case 3: Different elements
    std::vector<int> diff = {1, 2, 3, 4, 6}; // Last element differs
    EXPECT_FALSE(core::equal(same1.begin(), same1.end(), diff.begin()));

    // Case 6: Custom predicate - case insensitive string comparison
    std::string str1 = "Hello";
    std::string str2 = "hello";
    auto case_insensitive = [](char a, char b) { return std::tolower(a) == std::tolower(b); };
    EXPECT_TRUE(core::equal(str1.begin(), str1.end(), str2.begin(), case_insensitive));

    // Case 7: Custom predicate - absolute value comparison
    std::vector<int> abs1 = {1, -2, 3, -4, 5};
    std::vector<int> abs2 = {1, 2, 3, 4, 5};
    auto abs_compare = [](int a, int b) { return std::abs(a) == std::abs(b); };
    EXPECT_TRUE(core::equal(abs1.begin(), abs1.end(), abs2.begin(), abs_compare));

    // Case 8: Custom predicate - floating-point comparison with tolerance
    std::vector<double> float1 = {1.00001, 2.00002, 3.00003};
    std::vector<double> float2 = {1.00002, 2.00003, 3.00004};
    auto float_tolerance = [](double a, double b) { return std::fabs(a - b) < 0.0001; };
    EXPECT_TRUE(core::equal(float1.begin(), float1.end(), float2.begin(), float_tolerance));

    // Case 9: Mismatch with custom predicate
    std::vector<int> mismatch1 = {1, 2, 3, 4};
    std::vector<int> mismatch2 = {1, 2, 3, 5};
    EXPECT_FALSE(core::equal(mismatch1.begin(), mismatch1.end(), mismatch2.begin(), abs_compare));

    // Case 10: Non-random access container (std::list)
    std::list<int> list1 = {10, 20, 30, 40, 50};
    std::list<int> list2 = {10, 20, 30, 40, 50};
    EXPECT_TRUE(core::equal(list1.begin(), list1.end(), list2.begin()));

    // Case 11: Non-random access container with mismatch
    std::list<int> list3 = {10, 20, 30, 40, 60};  // Last element differs
    EXPECT_FALSE(core::equal(list1.begin(), list1.end(), list3.begin()));

    // Case 12: Non-random access container with a custom predicate (check even/odd equivalence)
    std::list<int> list4 = {4, 6, 9, 10, 12};
    std::list<int> list5 = {2, 4, 5, 8, 10};
    auto odd_even_equivalent = [](int a, int b) { return (a % 2) == (b % 2); };
    EXPECT_TRUE(core::equal(list4.begin(), list4.end(), list5.begin(), odd_even_equivalent));
}

TEST(AlgorithmTests, Equal) {
    // Tests for (safe) core::Equal() overload

    // Case 1: Empty ranges (should be equal)
    std::vector<int> empty1, empty2;
    EXPECT_TRUE(core::equal(empty1.begin(), empty1.end(), empty2.begin(), empty2.end()));

    // Case 2: Equal ranges
    std::vector<int> same1 = {1, 2, 3, 4, 5};
    std::vector<int> same2 = {1, 2, 3, 4, 5};
    EXPECT_TRUE(core::equal(same1.begin(), same1.end(), same2.begin(), same2.end()));

    // Case 3: Different elements
    std::vector<int> diff = {1, 2, 3, 4, 6}; // Last element differs
    EXPECT_FALSE(core::equal(same1.begin(), same1.end(), diff.begin(), diff.end()));

    // Case 4: Different lengths (should return false immediately for random access)
    std::vector<int> longer = {1, 2, 3, 4, 5, 6};
    std::vector<int> shorter = {1, 2, 3, 4};
    EXPECT_FALSE(core::equal(same1.begin(), same1.end(), longer.begin(), longer.end()));
    EXPECT_FALSE(core::equal(same1.begin(), same1.end(), shorter.begin(), shorter.end()));

    // Case 5: Non-random access container (std::list)
    std::list<int> list1 = {10, 20, 30, 40, 50};
    std::list<int> list2 = {10, 20, 30, 40, 50};
    EXPECT_TRUE(core::equal(list1.begin(), list1.end(), list2.begin(), list2.end()));

    // Case 6: Non-random access container with different lengths (should return false)
    std::list<int> list3 = {10, 20, 30, 40};
    EXPECT_FALSE(core::equal(list1.begin(), list1.end(), list3.begin(), list3.end()));

    // Case 7: Custom predicate - case insensitive string comparison
    std::string str1 = "Hello";
    std::string str2 = "hello";
    auto case_insensitive = [](char a, char b) { return std::tolower(a) == std::tolower(b); };
    EXPECT_TRUE(core::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), case_insensitive));

    // Case 8: Custom predicate - absolute value comparison
    std::vector<int> abs1 = {1, -2, 3, -4, 5};
    std::vector<int> abs2 = {1, 2, 3, 4, 5};
    auto abs_compare = [](int a, int b) { return std::abs(a) == std::abs(b); };
    EXPECT_TRUE(core::equal(abs1.begin(), abs1.end(), abs2.begin(), abs2.end(), abs_compare));

    // Case 9: Custom predicate - floating-point comparison with tolerance
    std::vector<double> float1 = {1.00001, 2.00002, 3.00003};
    std::vector<double> float2 = {1.00002, 2.00003, 3.00004};
    auto float_tolerance = [](double a, double b) { return std::fabs(a - b) < 0.0001; };
    EXPECT_TRUE(core::equal(float1.begin(), float1.end(), float2.begin(), float2.end(), float_tolerance));

    // Case 10: Non-random access container with custom predicate (check even/odd equivalence)
    std::list<int> list4 = {4, 6, 9, 10, 12};
    std::list<int> list5 = {2, 4, 5, 8, 10};
    auto odd_even_equivalent = [](int a, int b) { return (a % 2) == (b % 2); };
    EXPECT_TRUE(core::equal(list4.begin(), list4.end(), list5.begin(), list5.end(), odd_even_equivalent));

    // Case 11: Verify optimizations for random access
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::vector<int> vec2 = {1, 2, 3, 4};  // Shorter than vec1
    std::vector<int> vec3 = {1, 2, 3, 4, 5, 6}; // Longer than vec1
    int cnt = 0;
    auto counter = [&](int a, int b) { 
        cnt++;
        return a == b;
    };
    EXPECT_FALSE(core::equal(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), counter));
    EXPECT_FALSE(core::equal(vec1.begin(), vec1.end(), vec3.begin(), vec3.end(), counter));
    EXPECT_EQ(cnt, 0); // should fail early, never check predicate
}

TEST(AlgorithmTests, Equal_constexpr) {
    constexpr std::array<int,3> a1{1,2,3};
    constexpr std::array<int,3> a2{1,2,4};
    static_assert(core::equal(a1.begin(), a1.end(), a1.begin()));
    static_assert(core::equal(a1.begin(), a1.end(), a1.begin(), a1.end()));
    static_assert(core::equal(a1.begin(), a1.end(), a1.begin(),
                  [](int a, int b){return a == b;}));
    static_assert(core::equal(a1.begin(), a1.end(), a1.begin(), a1.end(),
                  [](int a, int b){return a == b;}));

    static_assert(!core::equal(a1.begin(), a1.end(), a2.begin(), a2.end()));
    static_assert(!core::equal(a1.begin(), a1.end(), a2.begin(), a2.begin()+1));
    static_assert(!core::equal(a1.begin(), a1.end(), a2.begin(), a2.end(),
                   [](int a, int b){return a == b;}));
    static_assert(!core::equal(a1.begin(), a1.end(), a2.begin(), a2.begin()+1,
                   [](int a, int b){return a == b;}));
}

