#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <array>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>

using std::vector;
using std::array;
using std::list;

constexpr bool iseven(int i) { return i % 2 == 0; }
constexpr bool isodd(int i) { return !iseven(i); }

struct MoveOnly {
    int data;
    bool moved;
    constexpr MoveOnly(int value) : data(value), moved(false) {}
    MoveOnly(const MoveOnly&) = delete;
    constexpr MoveOnly(MoveOnly&& m) : data (m.data), moved(false) { m.moved = true; }
    MoveOnly& operator=(const MoveOnly&) = delete;
    constexpr MoveOnly& operator=(MoveOnly&& m) {
        if (this != &m) {
            data = m.data;
            moved = false;
            m.moved = true;
        }
        return *this;
    }
};

TEST(AlgorithmTests, Copy) {
    const vector<int> v1{1,2,3,4};
    vector<int> v2(v1.size());
    auto it = core::copy(v1.begin(), v1.end(), v2.begin());
    EXPECT_EQ(it, v2.end());
    EXPECT_EQ(v1, v2);

    vector<int> e1{};
    vector<int> e2{};
    it = core::copy(e1.begin(), e1.end(), e2.begin());
    EXPECT_EQ(it, e2.begin());
    EXPECT_TRUE(e2.empty());

    // Non-random access
    std::list<int> list_src = {10, 20, 30, 40, 50};
    std::list<int> list_dest(5);
    auto it2 = core::copy(list_src.begin(), list_src.end(), list_dest.begin());
    EXPECT_EQ(it2, list_dest.end());
    EXPECT_EQ(list_dest, list_src);

    std::string str1 = "$xyz$";
    std::string str2 = "AB___CD";
    EXPECT_EQ(core::copy(str1.begin()+1, str1.begin()+4, str2.begin()+2),
              str2.begin()+5);
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
    auto it = core::copy_if(v1.begin(), v1.end(), v2.begin(), istrue);
    EXPECT_EQ(it, v2.end());
    EXPECT_EQ(v1, v2);

    // copy subset
    vector<int> even(3,0);
    it = core::copy_if(v1.begin(), v1.end(), even.begin(), iseven);
    EXPECT_EQ(it, even.begin()+2);
    EXPECT_EQ(even, (vector<int>{2,4,0}) );

    vector<int> e1{};
    vector<int> e2{};
    it = core::copy_if(e1.begin(), e1.end(), e2.begin(), istrue);
    EXPECT_EQ(it, e2.begin());
    EXPECT_TRUE(e2.empty());

    // Don't copy
    vector<int> v3(v1.size(), 0);
    it = core::copy_if(v1.begin(), v1.end(), v3.begin(), isfalse);
    EXPECT_EQ(it, v3.begin());
    EXPECT_EQ(v3, (vector<int>{0,0,0,0}) );

    // copy into empty
    vector<int> e3{};
    it = core::copy_if(v1.begin(), v1.end(), e3.begin(), isfalse);
    EXPECT_EQ(it, e3.begin());
    EXPECT_TRUE(e3.empty());

    // Non-random access
    list<int> list_src = {1, 2, 3, 4, 5};
    list<int> list_dest = {0,0,0,0,0};
    auto it2 = core::copy_if(list_src.begin(), list_src.end(), list_dest.begin(), iseven);
    EXPECT_EQ(it2, std::next(list_dest.begin(),2));
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

TEST(AlgorithmTests, CopyN) {
    // different iterators
    const vector<int> v1{1,2,3,4};
    list<int> l1{1,1,1,1,1};
    auto lit = core::copy_n(v1.begin(), 3, l1.begin());
    EXPECT_EQ(lit, std::next(l1.begin(),3));
    EXPECT_EQ(l1, (list<int>{1,2,3,1,1}));
    const list<int> l2{1,2,3,4};
    vector<int> v2{1,1,1,1,1};
    auto vit = core::copy_n(l2.begin(), 3, v2.begin());
    EXPECT_EQ(vit, std::next(v2.begin(),3));
    EXPECT_EQ(v2, (vector<int>{1,2,3,1,1}));

    // n is 0 or -1
    vector<int> v3{0};
    vit = core::copy_n(v1.begin(), 0, v3.begin());
    EXPECT_EQ(vit, v3.begin());
    EXPECT_EQ(v3, (vector<int>{0}));
    vit = core::copy_n(v1.begin(), -1, v3.begin());
    EXPECT_EQ(vit, v3.begin());
    EXPECT_EQ(v3, (vector<int>{0}));
}

TEST(AlgorithmTests, CopyN_constexpr) {
    constexpr array<int,4> a1{1,2,3,4};
    constexpr array<int,4> a2 = [&]() {
        array<int,4> tmp{0};
        core::copy_n(a1.begin(), 2, tmp.begin()+1);
        return tmp;
    }();
    constexpr array<int,4> a3{0,1,2,0};
    static_assert(a2 == a3);
}

TEST(AlgorithmTests, CopyBackward) {
    const vector<int> v1{1,2,3,4};
    list<int> l1{0,0,0,0};
    auto lit = core::copy_backward(v1.begin(),std::prev(v1.end()), l1.end());
    EXPECT_EQ(lit, std::next(l1.begin()));
    EXPECT_EQ(l1, (list<int>{0,1,2,3}));

    const list<int> l2{1,2,3,4};
    vector<int> v2{0,0,0,0};
    auto vit = core::copy_backward(l2.begin(),std::prev(l2.end()), v2.end());
    EXPECT_EQ(vit, std::next(v2.begin()));
    EXPECT_EQ(v2, (vector<int>{0,1,2,3}));

    list<int> e1{};
    vector<int> e2{};
    EXPECT_EQ(core::copy_backward(e1.begin(), e1.end(), e2.end()), e2.end());
}

TEST(AlgorithmTests, CopyBackward_constexpr) {
    constexpr array<int,4> a1{1,2,3,4};
    constexpr array<int,5> a2 = [&]() {
        array<int,5> tmp{0};
        core::copy_backward(a1.begin(), a1.end(), tmp.end());
        return tmp;
    }();
    constexpr array<int,5> a3{0,1,2,3,4};
    static_assert(a2 == a3);
}

TEST(AlgorithmTests, Move) {
    vector<std::string> src_str = {"one", "two", "three", "four"};
    vector<std::string> dest_str = {"a", "b", "c", "d", "e"};
    auto sit = core::move(src_str.begin(), src_str.begin()+3, dest_str.begin());
    EXPECT_EQ(sit, dest_str.begin()+3);
    EXPECT_EQ(dest_str, (vector<std::string>{"one", "two", "three", "d", "e"}));

    list<MoveOnly> l1;
    vector<MoveOnly> v1;
    for (int i = 0; i < 3; i++) {
        l1.emplace_back(i);
        v1.emplace_back(-1);
    }
    auto vit = core::move(l1.begin(), l1.end(), v1.begin());
    EXPECT_EQ(vit, v1.end());
    auto lit = l1.begin();
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(v1[i].data, i);
        EXPECT_FALSE(v1[i].moved);
        EXPECT_TRUE(lit->moved);
        ++lit;
    }
}

TEST(AlgorithmTests, Move_constexpr) {
    constexpr bool valid = []() constexpr {
        array<MoveOnly,2> a1{MoveOnly(1), MoveOnly(2)};
        array<MoveOnly,2> a2{MoveOnly(-1), MoveOnly(-1)};
        bool b = (core::move(a1.begin(), a1.end(), a2.begin()) == a2.end());
        for (int i = 0; i < 2; i++) {
            b &= (a2[i].data == i+1);
            b &= (a2[i].moved == false);
            b &= (a1[i].moved == true);
        }
        return b;
    }();
    static_assert(valid);
}

TEST(AlgorithmTests, MoveBackward) {
    vector<std::string> src_str = {"one", "two", "three", "four"};
    vector<std::string> dest_str = {"a", "b", "c", "d", "e"};
    auto sit = core::move_backward(src_str.begin(), src_str.begin()+3, dest_str.end());
    EXPECT_EQ(sit, dest_str.end()-3);
    EXPECT_EQ(dest_str, (vector<std::string>{"a", "b", "one", "two", "three"}));

    list<MoveOnly> l1;
    vector<MoveOnly> v1;
    for (int i = 0; i < 3; i++) {
        l1.emplace_back(i);
        v1.emplace_back(-1);
    }
    v1.emplace_back(-1); // v1={-1, -1, -1, -1}

    auto vit = core::move_backward(l1.begin(), l1.end(), v1.end());
    EXPECT_EQ(vit, v1.begin()+1);
    auto lit = l1.begin();
    EXPECT_EQ(v1[0].data, -1); // v1={-1, 0, 1, 2}
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(v1[i+1].data, i);
        EXPECT_FALSE(v1[i].moved);
        EXPECT_TRUE(lit->moved);
        ++lit;
    }
}

TEST(AlgorithmTests, MoveBackward_constexpr) {
    constexpr bool valid = []() constexpr {
        array<MoveOnly,2> a1{MoveOnly(1), MoveOnly(2)};
        array<MoveOnly,3> a2{MoveOnly(-1), MoveOnly(-1), MoveOnly(-1)};
        bool b = (core::move_backward(a1.begin(), a1.end(), a2.end()) == a2.begin()+1);
        b &= (a2[0].data == -1);
        for (int i = 1; i <= 2; i++) {
            b &= (a2[i].data == i);
            b &= (a2[i].moved == false);
            b &= (a1[i-1].moved == true);
        }
        return b;
    }();
    static_assert(valid);
}
