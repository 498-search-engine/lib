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
#include <utility>

using std::vector;
using std::array;
using std::list;

TEST(AlgorithmTests, LowerBound) {
    const vector<int> v1{1,2,3,4,5};
    for (int i = 1; i <= 5; i++) {
        auto it = core::lower_bound(v1.begin(), v1.end(), i);
        EXPECT_EQ(it, v1.begin()+(i-1));
    }
    auto it = core::lower_bound(v1.begin(), v1.end(), 0);
    EXPECT_EQ(it, v1.begin());
    it = core::lower_bound(v1.begin(), v1.end(), 6);
    EXPECT_EQ(it, v1.end());

    const vector<int> v2{5,4,3,2,1};
    for (int i = 1; i <= 5; i++) {
        auto it2 = core::lower_bound(v2.begin(), v2.end(), i, std::greater{});
        EXPECT_EQ(it2, v2.begin()+(5-i));
    }
    auto it2 = core::lower_bound(v2.begin(), v2.end(), 6, std::greater{});
    EXPECT_EQ(it2, v2.begin());
    it2 = core::lower_bound(v2.begin(), v2.end(), 0, std::greater{});
    EXPECT_EQ(it2, v2.end());

    // Non random access
    const list<int> l1{1,2,3,4,5};
    for (int i = 1; i <= 5; i++) {
        auto it3 = core::lower_bound(l1.begin(), l1.end(), i, std::less{});
        EXPECT_EQ(it3, std::next(l1.begin(), i-1));
    }
    auto it3 = core::lower_bound(l1.begin(), l1.end(), 0);
    EXPECT_EQ(it3, l1.begin());
    it3 = core::lower_bound(l1.begin(), l1.end(), 6);
    EXPECT_EQ(it3, l1.end());

    // empty
    vector<int> v3{};
    EXPECT_EQ(core::lower_bound(v3.begin(), v3.end(), 0), v3.end());
}

TEST(AlgorithmTests, LowerBound_constexpr) {
    constexpr array<int,4> a{1,2,3,4};
    static_assert(core::lower_bound(a.begin(), a.end(), 1) == a.begin());
    static_assert(core::lower_bound(a.begin(), a.end(), 1, std::less{}) == a.begin());
}

TEST(AlgorithmTests, UpperBound) {
    const vector<int> v1{1,2,3,4,5};
    for (int i = 1; i <= 5; i++) {
        auto it = core::upper_bound(v1.begin(), v1.end(), i);
        EXPECT_EQ(it, v1.begin()+i);
    }
    auto it = core::upper_bound(v1.begin(), v1.end(), 0);
    EXPECT_EQ(it, v1.begin());
    it = core::upper_bound(v1.begin(), v1.end(), 6);
    EXPECT_EQ(it, v1.end());

    const vector<int> v2{5,4,3,2,1};
    for (int i = 1; i <= 5; i++) {
        auto it2 = core::upper_bound(v2.begin(), v2.end(), i, std::greater{});
        std::cout << "i: " << i << " idx: " << (it2 - v2.begin()) << "\n";
        EXPECT_EQ(it2, v2.begin()+(5-i+1));
    }
    auto it2 = core::upper_bound(v2.begin(), v2.end(), 6, std::greater{});
    EXPECT_EQ(it2, v2.begin());
    it2 = core::upper_bound(v2.begin(), v2.end(), 0, std::greater{});
    EXPECT_EQ(it2, v2.end());

    // Non random access
    const list<int> l1{1,2,3,4,5};
    for (int i = 1; i <= 5; i++) {
        auto it3 = core::upper_bound(l1.begin(), l1.end(), i, std::less{});
        EXPECT_EQ(it3, std::next(l1.begin(), i));
    }
    auto it3 = core::upper_bound(l1.begin(), l1.end(), 0);
    EXPECT_EQ(it3, l1.begin());
    it3 = core::upper_bound(l1.begin(), l1.end(), 6);
    EXPECT_EQ(it3, l1.end());

    // empty
    vector<int> v3{};
    EXPECT_EQ(core::upper_bound(v3.begin(), v3.end(), 0), v3.end());
}

TEST(AlgorithmTests, UpperBound_constexpr) {
    constexpr array<int,4> a{1,2,3,4};
    static_assert(core::upper_bound(a.begin(), a.end(), 1) == a.begin()+1);
    static_assert(core::upper_bound(a.begin(), a.end(), 1, std::less{}) == a.begin()+1);
}

TEST(AlgorithmTests, BinarySearch) {
    vector<int> v1{1,2,3,4,7};
    for (int i = 1; i <= 4; i++)
        EXPECT_TRUE(core::binary_search(v1.begin(), v1.end(), i));
    EXPECT_FALSE(core::binary_search(v1.begin(), v1.end(),0));
    EXPECT_FALSE(core::binary_search(v1.begin(), v1.end(),5));
    EXPECT_FALSE(core::binary_search(v1.begin(), v1.end(),8));

    vector<int> v2{7,4,3,2,1};
    for (int i = 1; i <= 4; i++)
        EXPECT_TRUE(core::binary_search(v2.begin(), v2.end(), i, std::greater{}));
    EXPECT_FALSE(core::binary_search(v2.begin(), v2.end(),0, std::greater{}));
    EXPECT_FALSE(core::binary_search(v2.begin(), v2.end(),5, std::greater{}));
    EXPECT_FALSE(core::binary_search(v2.begin(), v2.end(),8, std::greater{}));

    vector<int> e{};
    EXPECT_FALSE(core::binary_search(e.begin(),e.end(),0));
}

TEST(AlgorithmTests, BinarySearch_constexpr) {
    constexpr array<int,4> a{1,2,3,4};
    static_assert(core::binary_search(a.begin(),a.end(),2));
    static_assert(core::binary_search(a.begin(),a.end(),2,std::less{}));
}
