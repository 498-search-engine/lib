#include "core/algorithm.h"

#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <iterator>
#include <utility>
#include <initializer_list>
#include <functional>

using std::vector;
using std::array;
using std::list;
template <typename T> using slist = std::forward_list<T>;

TEST(AlgorithmTests, Max) {
    int i1 = 1, i2 = 2;
    EXPECT_EQ(core::max(i1, i2), i2);
    EXPECT_EQ(core::max(-1,-2),-1);

    EXPECT_EQ(core::max(i1, i2, std::greater{}), i1);
    EXPECT_EQ(core::max(1,2, std::greater{}), 1);

    EXPECT_EQ(core::max({1,2,3,4}), 4);
    EXPECT_EQ(core::max({1,2,3,4}, std::greater{}), 1);
}


TEST(AlgorithmTests, Max_constexpr) {
    static_assert(core::max(1,2) == 2);
    static_assert(core::max(1,2,std::greater{}) == 1);
    static_assert(core::max({1,2}) == 2);
    static_assert(core::max({1,2},std::greater{}) == 1);
}

TEST(AlgorithmTests, MaxElement) {
    vector<int> v{1,2,3,4,5};
    list<int> l{1,2,3,4,5};
    slist<int> s{1,2,3,4,5};

    EXPECT_EQ(core::max_element(v.begin(),v.end()), std::next(v.begin(),4));
    EXPECT_EQ(core::max_element(v.begin(),v.end(),std::greater{}),v.begin());
    EXPECT_EQ(core::max_element(l.begin(),l.end()), std::next(l.begin(),4));
    EXPECT_EQ(core::max_element(l.begin(),l.end(),std::greater{}),l.begin());
    EXPECT_EQ(core::max_element(s.begin(),s.end()), std::next(s.begin(),4));
    EXPECT_EQ(core::max_element(s.begin(),s.end(),std::greater{}),s.begin());

    vector<int> empty{};
    EXPECT_EQ(core::max_element(empty.begin(), empty.end()), empty.end());
    EXPECT_EQ(core::max_element(empty.begin(), empty.end(), std::greater{}), empty.end());
}

TEST(AlgorithmTests, MaxElement_constexpr) {
    constexpr array<int,4> a{1,2,3,4};
    static_assert(core::max_element(a.begin(),a.end()) == a.begin()+3);
    static_assert(core::max_element(a.begin(),a.end(),std::less{}) == a.begin()+3);
}