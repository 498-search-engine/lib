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
#include <forward_list>
#include <initializer_list>
#include <utility>

#include <algorithm>

using std::vector;
using std::array;
using std::list;
template <typename T> using slist = std::forward_list<T>;

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

    // Move overlapping ranges
    vector<MoveOnly> v2; // {0,1,2,3,4}
    for (int i = 0; i < 5; i++)
        v2.emplace_back(i);
    vit = core::move(v2.begin()+2,v2.end(),v2.begin());
    EXPECT_EQ(vit, v2.begin()+3); // {2,3,4,_,_}
    for (int i = 0; i < 3; i++) {
        EXPECT_FALSE(v2[i].moved);
        EXPECT_EQ(v2[i].data, i + 2);
    }
    for (int i = 3; i < 5; i++) {
        EXPECT_TRUE(v2[i].moved);
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

TEST(AlgorithmTests, Fill) {
    list<int> v{0,0,0,0,0};
    core::fill(v.begin(), v.end(), 1);
    EXPECT_EQ(v, (list<int>{1,1,1,1,1}));
    core::fill(v.begin(), v.begin(), 2);
    EXPECT_EQ(v, (list<int>{1,1,1,1,1}));
}

TEST(AlgorithmTests, Fill_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> v{0,0,0,0,0};
        core::fill(v.begin(), v.end(), 1);
        return v;
    }();
    static_assert(a == (array<int,5>{1,1,1,1,1}));
}

TEST(AlgorithmTests, FillN) {
    list<int> v{0,0,0,0,0};
    EXPECT_EQ(core::fill_n(v.begin(), 3, 1), std::next(v.begin(),3));
    EXPECT_EQ(core::fill_n(v.begin(), 0, 2), v.begin()); // do nothing
    EXPECT_EQ(core::fill_n(v.begin(), -1, 2), v.begin()); // do nothing
    EXPECT_EQ(v, (list<int>{1,1,1,0,0}));
}

TEST(AlgorithmTests, FillN_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> v{0,0,0,0,0};
        core::fill_n(v.begin(), 4, 1);
        return v;
    }();
    static_assert(a == (array<int,5>{1,1,1,1,0}));
}

TEST(AlgorithmTests, Transform) {
    const list<int> src = {1, 2, 3, 4};
    slist<int> dest(src.size());
    auto slit = core::transform(src.begin(), src.end(), dest.begin(),
                                [](int x) { return x * x; });
    EXPECT_EQ(slit, dest.end());
    EXPECT_EQ(dest, (slist<int>{1, 4, 9, 16}));

    const slist<int> src2 = {0, 2, 0, 2};
    vector<int> dest2(src.size());
    auto vit = core::transform(src.begin(), src.end(), src2.begin(), dest2.begin(),
                               [](int a, int b) { return a * b; });
    EXPECT_EQ(vit, dest2.end());
    EXPECT_EQ(dest2, (vector<int>{0, 4, 0, 8}));
}

TEST(AlgorithmTests, Transform_constexpr) {
    constexpr array<int,4> src = {1, 2, 3, 4};
    constexpr array<int,4> da = [&](){
        array<int,4> dest;
        core::transform(src.begin(), src.end(), dest.begin(),
                        [](int x) { return x * x; });
        return dest;
    }();
    static_assert(da == (array<int,4>{1, 4, 9, 16}));

    constexpr array<int,4> src2 = {0, 2, 0, 2};
    constexpr array<int,4> da2 = [&](){
        array<int,4> dest2;
        core::transform(src.begin(), src.end(), src2.begin(), dest2.begin(),
                        [](int a, int b) { return a * b; });
        return dest2;
    }();
    static_assert(da2 == (array<int,4>{0, 4, 0, 8}));
}

TEST(AlgorithmTests, Generate) {
    vector<int> vec(5);
    list<int> lst(5);
    int counter = 0;
    auto generator = [&counter]() { return counter++; };
    core::generate(vec.begin(), vec.end(), generator);
    counter = 0;
    core::generate(lst.begin(), lst.end(), generator);
    EXPECT_EQ(vec, (vector<int>{0,1,2,3,4}));
    EXPECT_EQ(lst, (list<int>{0,1,2,3,4}));
}

TEST(AlgorithmTests, Generate_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> a1;
        int i = 0;
        core::generate(a1.begin(), a1.end(), [&](){return i++;});
        return a1;
    }();
    static_assert(a == array<int,5>{0,1,2,3,4});
}

TEST(AlgorithmTests, GenerateN) {
    vector<int> vec(5,-1);
    list<int> lst(5,-1);
    int counter = 0;
    auto generator = [&counter]() { return counter++; };
    EXPECT_EQ(core::generate_n(vec.begin(), 4, generator), vec.begin()+4);
    counter = 0;
    EXPECT_EQ(core::generate_n(lst.begin(), 4, generator), std::next(lst.begin(),4));
    EXPECT_EQ(vec, (vector<int>{0,1,2,3,-1}));
    EXPECT_EQ(lst, (list<int>{0,1,2,3,-1}));
}

TEST(AlgorithmTests, GenerateN_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> a1{-1,-1,-1,-1,-1};
        int i = 0;
        core::generate_n(a1.begin(), 4, [&](){return i++;});
        return a1;
    }();
    static_assert(a == array<int,5>{0,1,2,3,-1});
}

TEST(AlgorithmTests, Remove) {
    list<int> l1{1,2,3,2,4,5};
    auto lend = core::remove(l1.begin(), l1.end(), -1);
    EXPECT_EQ(lend, l1.end());
    EXPECT_EQ(l1, (list<int>{1,2,3,2,4,5}));
    lend = core::remove(l1.begin(), l1.end(), 2);
    EXPECT_EQ(lend, std::next(l1.begin(),4));
    list<int> l3(l1.begin(), lend);
    EXPECT_EQ(l3, (list<int>{1,3,4,5}));
}

TEST(AlgorithmTests, Remove_constexpr) {
    constexpr array<int,4> a = []() {
        array<int,6> a1{1,2,3,2,4,5};
        auto it = core::remove(a1.begin(), a1.end(), 2);
        return array<int,4>{a1[0], a1[1], a1[2], a1[3]};
    }();
    static_assert(a == array<int,4>{1,3,4,5});
}

TEST(AlgorithmTests, RemoveIf) {
    list<int> l1{1,2,3,4,5,6};
    auto lend = core::remove_if(l1.begin(), l1.end(), [](int i){return false; });
    EXPECT_EQ(lend, l1.end());
    EXPECT_EQ(l1, (list<int>{1,2,3,4,5,6}));
    lend = core::remove_if(l1.begin(), l1.end(), iseven);
    EXPECT_EQ(lend, std::next(l1.begin(),3));
    list<int> l3(l1.begin(), lend);
    EXPECT_EQ(l3, (list<int>{1,3,5}));
}

TEST(AlgorithmTests, RemoveIf_constexpr) {
    constexpr array<int,3> a = []() {
        array<int,6> a1{1,2,3,2,4,5};
        core::remove_if(a1.begin(), a1.end(), iseven);
        return array<int,3>{a1[0], a1[1], a1[2]};
    }();
    static_assert(a == array<int,3>{1,3,5});
}

TEST(AlgorithmTests, RemoveCopy) {
    list<int> source = {1, 2, 3, 2, 4};
    list<int> dest(source.size(), 0);
    auto new_end = core::remove_copy(source.begin(), source.end(), dest.begin(), 2);
    list<int> expected = {1, 3, 4};
    EXPECT_EQ(std::distance(dest.begin(), new_end), expected.size());
    auto i1 = dest.begin(), i2 = expected.begin();
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(*i1, *i2);
        ++i1, ++i2;
    }

    std::vector<int> source2{};
    std::vector<int> dest2{};
    auto new_end2 = core::remove_copy(source2.begin(), source2.end(), dest2.begin(), 42);
    EXPECT_EQ(new_end2, dest2.begin());
}

TEST(AlgorithmTests, RemoveCopy_constexpr) {
    constexpr array<int, 5> source = {1, 2, 3, 2, 4};
    constexpr array<int, 5> a = [&]() {
        array<int,5> dest{0,0,0,0,0};
        core::remove_copy(source.begin(), source.end(), dest.begin(), 2);
        return dest;
    }();
    static_assert(a == array<int,5>{1,3,4,0,0});
}

TEST(AlgorithmTests, RemoveCopyIf) {
    list<int> source = {1, 2, 3, 4, 5};
    list<int> dest(source.size(), 0);
    auto is_even = [](int x) { return x % 2 == 0; };
    auto new_end = core::remove_copy_if(source.begin(), source.end(), dest.begin(), is_even);
    list<int> expected = {1, 3, 5};
    EXPECT_EQ(std::distance(dest.begin(), new_end), expected.size());
    auto i1 = dest.begin(), i2 = expected.begin();
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(*i1, *i2);
        ++i1, ++i2;
    }

    std::vector<int> source2 = {2, 4, 6, 8};
    std::vector<int> dest2(source.size(), 0);
    auto new_end2 = core::remove_copy_if(source2.begin(), source2.end(), dest2.begin(),
                                        [](int x) { return x % 2 == 0; });
    EXPECT_EQ(std::distance(dest2.begin(), new_end2), 0);
}

TEST(AlgorithmTests, RemoveCopyIf_constexpr) {
    constexpr array<int, 5> source = {1, 2, 3, 2, 4};
    constexpr array<int, 5> a = [&]() {
        array<int,5> dest{0,0,0,0,0};
        core::remove_copy_if(source.begin(), source.end(), dest.begin(), iseven);
        return dest;
    }();
    static_assert(a == array<int,5>{1,3,0,0,0});
}

TEST(AlgorithmTests, Replace) {
    list<int> l1{1,2,3,2,4,5,2};
    core::replace(l1.begin(), l1.end(), 2, 0);
    EXPECT_EQ(l1, (list<int>{1,0,3,0,4,5,0}));
}

TEST(AlgorithmTests, Replace_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> a1{1,2,3,4,5};
        core::replace(a1.begin(), a1.end(), 2, 0);
        return a1;
    }();
    static_assert(a == array<int,5>{1,0,3,4,5});
}

TEST(AlgorithmTests, ReplaceIf) {
    list<int> l1{1,2,3,4,5};
    core::replace_if(l1.begin(), l1.end(), iseven, 0);
    EXPECT_EQ(l1, (list<int>{1,0,3,0,5}));
}

TEST(AlgorithmTests, ReplaceIf_constexpr) {
    constexpr array<int,5> a = [](){
        array<int,5> a1{1,2,3,4,5};
        core::replace_if(a1.begin(), a1.end(), iseven, 0);
        return a1;
    }();
    static_assert(a == array<int,5>{1,0,3,0,5});
}

TEST(AlgorithmTests, ReplaceCopy) {
    const list<int> l1{1,2,3,2,4,5,2};
    list<int> l2(l1.size()+1, 0);
    auto it = core::replace_copy(l1.begin(), l1.end(), l2.begin(), 2, 0);
    EXPECT_EQ(it, std::next(l2.begin(), l1.size()));
    EXPECT_EQ(l2, (list<int>{1,0,3,0,4,5,0,0}));
}

TEST(AlgorithmTests, ReplaceCopy_constexpr) {
    constexpr array<int,6> src{1,2,3,2,5,2};
    constexpr array<int,6> a = [&](){
        array<int,6> a1{0,0,0,0,0,0};
        core::replace_copy(src.begin(), src.end(), a1.begin(), 2, 0);
        return a1;
    }();
    static_assert(a == array<int,6>{1,0,3,0,5,0});
}

TEST(AlgorithmTests, ReplaceCopyIf) {
    const list<int> l1{1,2,3,2,4,5,2};
    list<int> l2(l1.size()+1, -1);
    auto it = core::replace_copy_if(l1.begin(), l1.end(), l2.begin(), iseven, 0);
    EXPECT_EQ(it, std::next(l2.begin(), l1.size()));
    EXPECT_EQ(l2, (list<int>{1,0,3,0,0,5,0,-1}));
}

TEST(AlgorithmTests, ReplaceCopyIf_constexpr) {
    constexpr array<int,6> src{1,2,3,4,5,6};
    constexpr array<int,6> a = [&](){
        array<int,6> a1{0,0,0,0,0,0};
        core::replace_copy_if(src.begin(), src.end(), a1.begin(), iseven, 0);
        return a1;
    }();
    static_assert(a == array<int,6>{1,0,3,0,5,0});
}

TEST(AlgorithmTests, Swap) {
    MoveOnly a(1);
    MoveOnly b(2);
    core::swap(a,b);
    EXPECT_EQ(a.data, 2);
    EXPECT_EQ(b.data, 1);
    EXPECT_FALSE(a.moved || b.moved);

    MoveOnly c(3);
    core::swap(c,c);
    EXPECT_EQ(c.data, 3);
    EXPECT_FALSE(c.moved);
}

TEST(AlgorithmTests, Swap_constexpr) {
    constexpr bool valid = []() {
        MoveOnly a(1);
        MoveOnly b(2);
        core::swap(a,b);
        return (a.data == 2 && b.data == 1); 
    }();
    static_assert(valid);
}

TEST(AlgorithmTests, IterSwap) {
    vector<int> v{1,2};
    list<int> l{3,4};
    core::iter_swap(v.begin(), v.begin()+1);
    EXPECT_EQ(v, (vector<int>{2,1}));
    v = {1,2};
    core::iter_swap(v.begin(), l.begin());
    EXPECT_EQ(v, (vector<int>{3,2}));
    EXPECT_EQ(l, (list<int>{1,4}));
    array<MoveOnly,2> a{MoveOnly(1), MoveOnly(2)};
    core::iter_swap(a.begin(),a.begin()+1);
    EXPECT_TRUE(a[0].data == 2 && a[1].data == 1); 
}

TEST(AlgorithmTests, IterSwap_constexpr) {
    constexpr bool valid = []() {
        array<MoveOnly,2> a{MoveOnly(1), MoveOnly(2)};
        core::iter_swap(a.begin(),a.begin()+1);
        return (a[0].data == 2 && a[1].data == 1); 
    }();
    static_assert(valid);
}

TEST(AlgorithmTests, SwapRanges) {
    vector<char> v{'a', 'b', 'c', 'd', 'e'};
    list<char> l{'1', '2', '3', '4', '5'};
    auto it = core::swap_ranges(v.begin(), v.begin()+3, l.begin());
    EXPECT_EQ(it, std::next(l.begin(), 3));
    EXPECT_EQ(v, (vector<char>{'1','2','3','d','e'}));
    EXPECT_EQ(l, (list<char>{'a','b','c','4','5'}));

    array<MoveOnly,1> a{MoveOnly(1)};
    array<MoveOnly,1> b{MoveOnly(2)};
    core::swap_ranges(a.begin(),a.end(), b.begin());
    EXPECT_EQ(a[0].data, 2);
    EXPECT_EQ(b[0].data, 1);
    EXPECT_FALSE(a[0].moved || b[0].moved);
}

TEST(AlgorithmTests, SwapRanges_constexpr) {
    constexpr bool valid = []() {
        array<MoveOnly,1> a{MoveOnly(1)};
        array<MoveOnly,1> b{MoveOnly(2)};
        core::swap_ranges(a.begin(),a.end(), b.begin());
        return (a[0].data == 2 && b[0].data == 1);
    }();
    static_assert(valid);
}

TEST(AlgorithmTests, Reverse) {
    std::string str = "12345";
    core::reverse(str.begin(), str.begin());
    EXPECT_EQ(str, "12345");
    core::reverse(str.begin(), str.begin()+1);
    EXPECT_EQ(str, "12345");
    core::reverse(str.begin(), str.end());
    EXPECT_EQ(str, "54321");

    list<int> l{1,2,3};
    core::reverse(l.begin(), l.begin());
    core::reverse(l.begin(), std::next(l.begin(),1));
    EXPECT_EQ(l, (list<int>{1,2,3}));
    core::reverse(l.begin(), l.end());
    EXPECT_EQ(l, (list<int>{3,2,1}));
}

TEST(AlgorithmTests, Reverse_constexpr) {
    constexpr array<int,4> a = [](){
        array<int,4> a1{1,2,3,4};
        core::reverse(a1.begin(), a1.end());
        return a1;
    }();
    static_assert(a == (array<int,4>{4,3,2,1}));
}

TEST(AlgorithmTests, ReverseCopy) {
    std::string src = "12345";
    std::string dst = "$_____$";
    auto it = core::reverse_copy(src.begin(), src.end(), dst.begin()+1);
    EXPECT_EQ(it, dst.end()-1);
    EXPECT_EQ(dst, "$54321$");

    list<int> l1{1,2,3};
    list<int> l2{0,0,0};
    EXPECT_EQ(core::reverse_copy(l1.begin(), l1.begin(), l2.begin()), l2.begin());
    core::reverse_copy(l1.begin(), l1.end(), l2.begin());
    EXPECT_EQ(l2, (list<int>{3,2,1}));
}

TEST(AlgorithmTests, ReverseCopy_constexpr) {
    constexpr array<int,4> a = [](){
        array<int,4> a1{1,2,3,4};
        array<int,4> b1{0,0,0,0};
        core::reverse_copy(a1.begin(), a1.end(), b1.begin());
        return b1;
    }();
    static_assert(a == (array<int,4>{4,3,2,1}));
}

TEST(AlgorithmTests, Rotate) {
    const vector<std::tuple<std::initializer_list<int>,int,
                 std::initializer_list<int>>> cases = {
        {{1,2,3,4,5}, 0, {1,2,3,4,5}},
        {{1,2,3,4,5}, 1, {2,3,4,5,1}},
        {{1,2,3,4,5}, 2, {3,4,5,1,2}},
        {{1,2,3,4,5}, 3, {4,5,1,2,3}},
        {{1,2,3,4,5}, 4, {5,1,2,3,4}},
        {{1,2,3,4,5}, 5, {1,2,3,4,5}},
        {{}, 0, {}},
        {{1}, 0, {1}},
        {{1}, 1, {1}},
        {{1,2}, 1, {2,1}}
    };

    auto tester = [&]<class Container>() -> bool {
        for (const auto& [src, k, ans]: cases) {
            Container c = src, cans = ans;
            auto it = core::rotate(c.begin(), std::next(c.begin(),k), c.end());
            if(c != cans || std::distance(it, c.end()) != k)
                return false;
        }
        return true;
    };

    // Test random-access, bidirectional, and forward-only iterators
    EXPECT_TRUE(tester.template operator()<vector<int>>());
    EXPECT_TRUE(tester.template operator()<list<int>>());
    EXPECT_TRUE(tester.template operator()<std::forward_list<int>>());
}

TEST(AlgorithmTests, Rotate_constexpr) {
    constexpr auto arr = []() {
        array<int,4> a{1,2,3,4};
        core::rotate(a.begin(), a.begin()+1, a.end());
        return a;
    }();
    static_assert(arr == array<int,4>{2,3,4,1});
}

TEST(AlgorithmTests, ShiftLeft) {
    // Move semantics
    auto tester = [&]<class Container>() {
        Container v2; // {0,1,2,3,4}
        for (int i = 0; i < 5; i++)
            v2.emplace_back(i);
        auto vit = core::shift_left(v2.begin(),v2.end(),2);
        EXPECT_EQ(vit, std::next(v2.begin(),3)); // {2,3,4,_,_}
        for (int i = 0; i < 3; i++) {
            EXPECT_FALSE(std::next(v2.begin(),i)->moved);
            EXPECT_EQ(std::next(v2.begin(),i)->data, i + 2);
        }
        for (int i = 3; i < 5; i++) {
            EXPECT_TRUE(std::next(v2.begin(),i)->moved);
        }

        // invalid n
        Container c2;
        for(int i = 0; i < 2; i++) c2.emplace_back(i);
        EXPECT_EQ(core::shift_left(c2.begin(), c2.end(),0), c2.end()); // n = 0
        for (int i = 0; i < 2; i++) EXPECT_EQ(std::next(c2.begin(),i)->moved, false);
        EXPECT_EQ(core::shift_left(c2.begin(), c2.end(),2), c2.begin()); // n >= size
        for (int i = 0; i < 2; i++) EXPECT_EQ(std::next(c2.end(),i)->moved, false);
        EXPECT_EQ(core::shift_left(c2.begin(), c2.end(),3), c2.begin()); // n > size
        for (int i = 0; i < 2; i++) EXPECT_EQ(std::next(c2.begin(),i)->moved, false);

        for (int i = 0; i < 2; i++) EXPECT_EQ(std::next(c2.begin(),i)->data, i);
    };
    // Test different iterator types
    tester.template operator()<vector<MoveOnly>>();
    tester.template operator()<list<MoveOnly>>();
}

TEST(AlgorithmTests, ShiftLeft_constexpr) {
    constexpr auto arr = []() {
        array<int,4> a{1,2,3,4};
        core::shift_left(a.begin(),a.end(),1);
        a.back() = 0;
        return a;
    }();
    static_assert(arr == array<int,4>{2,3,4,0});
}
