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
#include <cmath>

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

TEST(AlgorithmTests, Min) {
    int i1 = 1, i2 = 2;
    EXPECT_EQ(core::min(i1, i2), i1);
    EXPECT_EQ(core::min(-1,-2),-2);

    EXPECT_EQ(core::min(i1, i2, std::greater{}), i2);
    EXPECT_EQ(core::min(1,2, std::greater{}), 2);

    EXPECT_EQ(core::min({1,2,3,4}), 1);
    EXPECT_EQ(core::min({1,2,3,4}, std::greater{}), 4);
}

TEST(AlgorithmTests, Min_constexpr) {
    static_assert(core::min(1,2) == 1);
    static_assert(core::min(1,2,std::greater{}) == 2);
    static_assert(core::min({1,2}) == 1);
    static_assert(core::min({1,2},std::greater{}) == 2);
}

TEST(AlgorithmTests, MinElement) {
    vector<int> v{5,4,3,2,1};
    list<int> l{5,4,3,2,1};
    slist<int> s{5,4,3,2,1};

    EXPECT_EQ(core::min_element(v.begin(),v.end()), std::next(v.begin(),4));
    EXPECT_EQ(core::min_element(v.begin(),v.end(),std::greater{}),v.begin());
    EXPECT_EQ(core::min_element(l.begin(),l.end()), std::next(l.begin(),4));
    EXPECT_EQ(core::min_element(l.begin(),l.end(),std::greater{}),l.begin());
    EXPECT_EQ(core::min_element(s.begin(),s.end()), std::next(s.begin(),4));
    EXPECT_EQ(core::min_element(s.begin(),s.end(),std::greater{}),s.begin());

    vector<int> empty{};
    EXPECT_EQ(core::max_element(empty.begin(), empty.end()), empty.end());
    EXPECT_EQ(core::max_element(empty.begin(), empty.end(), std::greater{}), empty.end());
}

TEST(AlgorithmTests, MinElement_constexpr) {
    constexpr array<int,4> a{4,3,2,1};
    static_assert(core::min_element(a.begin(),a.end()) == a.begin()+3);
    static_assert(core::min_element(a.begin(),a.end(),std::less{}) == a.begin()+3);
}

TEST(AlgorithmTests, Clamp) {
    EXPECT_EQ(core::clamp(1,1,1), 1);
    EXPECT_EQ(core::clamp(1,1,2), 1);
    EXPECT_EQ(core::clamp(0,1,2), 1);
    EXPECT_EQ(core::clamp(2,1,2), 2);
    EXPECT_EQ(core::clamp(3,1,2), 2);

    EXPECT_EQ(core::clamp(1,1,1,std::less{}), 1);
    EXPECT_EQ(core::clamp(1,1,2,std::less{}), 1);
    EXPECT_EQ(core::clamp(0,1,2,std::less{}), 1);
    EXPECT_EQ(core::clamp(2,1,2,std::less{}), 2);
    EXPECT_EQ(core::clamp(3,1,2,std::less{}), 2);
}

TEST(AlgorithmTests, Clamp_constexpr) {
    static_assert(core::clamp(1,0,2) == 1);
    static_assert(core::clamp(1,0,2,std::less{}) == 1);
}

TEST(AlgorithmTests, ClampRange) {
    slist<int> l1{1,2,3,4,5};
    core::clamp_range(l1.begin(), l1.end(), 2, 4);
    EXPECT_EQ(l1, (slist<int>{2,2,3,4,4}));

    auto abs = [](int a, int b){return std::abs(a) < std::abs(b); };
    slist<int> l2{-3,1,-2,3,-4,5};
    core::clamp_range(l2.begin(),l2.end(),2,3,abs);
    EXPECT_EQ(l2, (slist<int>{-3,2,-2,3,3,3}));

    vector<int> empty; // no error
    EXPECT_NO_THROW(core::clamp_range(empty.begin(), empty.end(), 0, 1));
}

TEST(AlgorithmTests, ClampRange_constexpr) {
    constexpr auto valid = [](){
        array<int,4> a{1,2,3,4};
        core::clamp_range(a.begin(),a.end(),2,3);
        return a == array<int,4>{2,2,3,3};
    }();
    static_assert(valid);
}
