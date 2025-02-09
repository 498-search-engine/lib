#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <array>
#include <list>
#include <algorithm>
#include <string>

using std::vector;
using std::array;
using std::list;

constexpr bool iseven(int i) { return i % 2 == 0; }
constexpr bool isodd(int i) { return !iseven(i); }

TEST(AlgorithmTests, Copy) {
    const vector<int> v1{1,2,3,4};
    vector<int> v2(v1.size());
    core::copy(v1.begin(), v1.end(), v2.begin());
    EXPECT_EQ(v1, v2);

    vector<int> e1{};
    vector<int> e2{};
    core::copy(e1.begin(), e1.end(), e2.begin());
    EXPECT_TRUE(e2.empty());

    // Non-random access
    std::list<int> list_src = {10, 20, 30, 40, 50};
    std::list<int> list_dest(5);
    core::copy(list_src.begin(), list_src.end(), list_dest.begin());
    EXPECT_EQ(list_dest, list_src);

    std::string str1 = "$xyz$";
    std::string str2 = "AB___CD";
    core::copy(str1.begin()+1, str1.begin()+4, str2.begin()+2);
    EXPECT_EQ(str2, "ABxyzCD");
}

TEST(AlgorithmTests, Copy_constexpr) {
    constexpr array<int,4> a1{1,2,3,4};
    constexpr array<int,4> a2 = [&]() {
        array<int,4> tmp{};
        core::copy(a1.begin(), a1.end(), tmp.begin());
        return tmp;
    }();
    static_assert(a1 == a2);
}

TEST(AlgorithmTests, CopyIf) {
    auto istrue = [](int i) -> bool {return true;};
    auto isfalse = [](int i) -> bool {return false;};

    const vector<int> v1{1,2,3,4};
    vector<int> v2(v1.size());
    core::copy_if(v1.begin(), v1.end(), v2.begin(), istrue);
    EXPECT_EQ(v1, v2);

    // copy subset
    vector<int> even(3,0);
    core::copy_if(v1.begin(), v1.end(), even.begin(), iseven);
    EXPECT_EQ(even, (vector<int>{2,4,0}) );

    vector<int> e1{};
    vector<int> e2{};
    core::copy_if(e1.begin(), e1.end(), e2.begin(), istrue);
    EXPECT_TRUE(e2.empty());

    // Don't copy
    vector<int> v3(v1.size(), 0);
    core::copy_if(v1.begin(), v1.end(), v2.begin(), isfalse);
    EXPECT_EQ(v3, (vector<int>{0,0,0,0}) );

    // copy into empty
    vector<int> e3{};
    core::copy_if(v1.begin(), v1.end(), e3.begin(), isfalse);
    EXPECT_TRUE(e3.empty());

    // Non-random access
    list<int> list_src = {1, 2, 3, 4, 5};
    list<int> list_dest = {0,0,0,0,0};
    core::copy_if(list_src.begin(), list_src.end(), list_dest.begin(), iseven);
    EXPECT_EQ(list_dest, (list<int>{2,4,0,0,0}));

    std::string str1 = "$x$$y$$z$$";
    std::string str2 = "AB___CD";
    core::copy_if(str1.begin()+1, str1.begin()+9, str2.begin()+2,
               [](char c){return c != '$';});
    EXPECT_EQ(str2, "ABxyzCD");
}

TEST(AlgorithmTests, CopyIf_constexpr) {
    constexpr array<int,4> a1{1,2,3,4};
    constexpr array<int,4> a2 = [&]() {
        array<int,4> tmp{0};
        core::copy_if(a1.begin(), a1.end(), tmp.begin()+1, isodd);
        return tmp;
    }();
    constexpr array<int,4> a3{0,1,3,0};
    static_assert(a2 == a3);
}