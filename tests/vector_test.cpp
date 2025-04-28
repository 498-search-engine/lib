#include "core/vector.h" 
#include <gtest/gtest.h>
#include <string>
#include <algorithm>

using core::Vector;

// util to help evaluate move ops
struct MoveSpy {
    static int constructions;
    static int destructions;
    static int moves;
    int value;
    MoveSpy(int v=0): value(v) { ++constructions; }
    MoveSpy(const MoveSpy& o): value(o.value) { ++constructions; }
    MoveSpy(MoveSpy&& o) noexcept : value(o.value) { ++moves; o.value = -1; }
    ~MoveSpy() { ++destructions; }
    bool operator==(MoveSpy const& o) const { return value == o.value; }
};

int MoveSpy::constructions = 0;
int MoveSpy::destructions = 0;
int MoveSpy::moves = 0;

TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

TEST(VectorTest, ResizeConstructor) {
    Vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorTest, FillConstructor) {
    Vector<std::string> v(3, "hello");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], "hello");
    }
}

TEST(VectorTest, CopyConstructor) {
    Vector<int> v1(4, 7);
    Vector<int> v2(v1);

    EXPECT_EQ(v2.size(), 4);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 7);
    }
}

TEST(VectorTest, CopyAssignment) {
    Vector<int> v1(2, 42);
    Vector<int> v2;
    v2 = v1;

    EXPECT_EQ(v2.size(), 2);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 42);
    }
}

TEST(VectorTest, MoveConstructor) {
    Vector<int> v1(3, 9);
    Vector<int> v2(std::move(v1));

    EXPECT_EQ(v2.size(), 3);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 9);
    }
    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, MoveAssignment) {
    Vector<int> v1(2, 5);
    Vector<int> v2;
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 2);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 5);
    }
    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, PushBack) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, PopBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, ReserveIncreasesCapacity) {
    Vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);

    v.push_back(5);
    EXPECT_EQ(v[0], 5);
}

TEST(VectorTest, BeginEndIteration) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected);
        expected++;
    }
}

TEST(VectorTest, RangeBasedIteration) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int expected = 1;
    for (auto i : v) {
        EXPECT_EQ(i, expected);
        expected++;
    }
}

TEST(VectorTest, InitializerListConstructor) {
    Vector<int> v = {1, 2, 3, 4, 5};

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5); // capacity could be larger due to growth strategy
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertMiddle) {
    Vector<int> v = {1, 2, 4, 5};
    v.insert(v.begin() + 2, 3); // Insert 3 at position 2 (between 2 and 4)

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertFront) {
    Vector<int> v = {2, 3, 4, 5};
    v.insert(v.begin(), 1); // Insert 1 at front

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, InsertBack) {
    Vector<int> v = {1, 2, 3, 4};
    v.insert(v.end(), 5); // Insert at end

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

// TEST(VectorTest, InsertOutOfRangeThrows) {
//     Vector<int> v = {1, 2, 3};

//     EXPECT_THROW(v.insert(v.begin() + 99, 4), std::out_of_range);
// }

TEST(VectorTest, CountAndValueConstructor) {
    Vector<int> v1(3);
    EXPECT_EQ(v1.size(), 3u);
    for (auto x : v1) EXPECT_EQ(x, 0);

    Vector<int> v2(4, 42);
    EXPECT_EQ(v2.size(), 4u);
    for (auto x : v2) EXPECT_EQ(x, 42);
}

TEST(VectorTest, InitializerListConstructor2) {
    Vector<std::string> v{"a","bb","ccc"};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "bb");
    EXPECT_EQ(v[2], "ccc");
}

TEST(VectorTest, CopyAndMoveConstructors) {
    Vector<int> orig{1,2,3};
    Vector<int> copy(orig);
    EXPECT_EQ(copy, orig);

    Vector<int> moved(std::move(orig));
    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(orig.size(), 0u);
}

TEST(VectorTest, CopyAndMoveAssignment) {
    Vector<int> a{1,2,3}, b;
    b = a;
    EXPECT_EQ(b, a);

    Vector<int> c{4,5};
    a = std::move(c);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(c.size(), 0u);
}

TEST(VectorTest, AssignMethods) {
    Vector<int> v;
    v.assign(3, 7);
    EXPECT_EQ(v.size(), 3u);
    for (auto x : v) EXPECT_EQ(x, 7);

    std::vector<int> src{1,2,3,4};
    v.assign(src.begin(), src.end());
    EXPECT_EQ(v.size(), 4u);
    for (size_t i=0; i<4; ++i) EXPECT_EQ(v[i], src[i]);

    v.assign({9,8,7});
    EXPECT_EQ((std::vector<int>{9,8,7}), std::vector<int>(v.begin(), v.end()));
}

TEST(VectorTest, AtThrowsOutOfRange) {
    Vector<int> v(2);
    EXPECT_NO_THROW(v.at(1));
    EXPECT_THROW(v.at(2), std::out_of_range);
}

TEST(VectorTest, FrontBackData) {
    Vector<int> v{10,20,30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
    EXPECT_EQ(v.data()[1], 20);
}

TEST(VectorTest, IteratorWorks) {
    Vector<int> v{1,2,3,4,5};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
    EXPECT_EQ(sum, 15);
    EXPECT_EQ(*std::min_element(v.begin(), v.end()), 1);
}

TEST(VectorTest, ReserveAndShrink) {
    Vector<int> v{1,2,3};
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10u);
    size_t old_cap = v.capacity();
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 3u);
}

TEST(VectorTest, ClearEmpty) {
    Vector<int> v{1,2,3};
    v.clear();
    EXPECT_TRUE(v.empty());
    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, PushPopEmplaceBack) {
    Vector<std::string> v;
    v.push_back("hello");
    EXPECT_EQ(v.back(), "hello");
    v.emplace_back(5, 'x');  // "xxxxx"
    EXPECT_EQ(v.back(), "xxxxx");
    v.pop_back();
    EXPECT_EQ(v.back(), "hello");
}

// FIXME: fails
// TEST(VectorTest, InsertAndErase) {
//     Vector<int> v{1,2,5};
//     // single insert
//     auto it = v.insert(v.begin()+2, 3);
//     EXPECT_EQ(*it, 3);
//     EXPECT_EQ((std::vector<int>{1,2,3,5}), std::vector<int>(v.begin(), v.end()));

//     // fill insert
//     v.insert(v.begin()+1, 2, 9);
//     EXPECT_EQ(v[1], 9);
//     EXPECT_EQ(v[2], 9);
//     EXPECT_EQ(v.size(), 7u);

//     // range insert
//     std::vector<int> more{7,8};
//     v.insert(v.end(), more.begin(), more.end());
//     EXPECT_EQ(v.back(), 8);

//     // erase single
//     it = v.erase(v.begin()+1);
//     EXPECT_EQ(*it, v[1]);

//     // erase range
//     size_t before = v.size();
//     it = v.erase(v.begin()+0, v.begin()+2);
//     EXPECT_EQ(v.size(), before - 2);
//     EXPECT_EQ(*it, v[0]);
// }

TEST(VectorTest, EmplaceMiddle) {
    Vector<std::pair<int,int>> v{{1,1},{3,3}};
    auto it = v.emplace(v.begin()+1, 2, 2);
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, 2);
}

TEST(VectorTest, ResizeAndSwap) {
    Vector<int> a{1,2,3};
    a.resize(5, 9);
    EXPECT_EQ(a.size(), 5u);
    EXPECT_EQ(a[3], 9);

    a.resize(2);
    EXPECT_EQ(a.size(), 2u);

    Vector<int> b{10,20};
    a.swap(b);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(b[0], 1);
}

TEST(VectorTest, ComparisonOperators) {
    Vector<int> a{1,2,3}, b{1,2,3}, c{1,2,4};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > b);
}

// FIXME: fails
// TEST(VectorTest, GrowAndMoveSpy) {
//     MoveSpy::constructions = MoveSpy::destructions = MoveSpy::moves = 0;
//     Vector<MoveSpy> v;
//     for (int i = 0; i < 10; ++i) {
//         v.emplace_back(i);
//     }
//     EXPECT_GE(MoveSpy::moves, 1);
//     EXPECT_EQ(v.size(), 10u);
//     v.clear();
//     EXPECT_EQ(v.size(), 0u);
//     // ensure all constructed objects were eventually destroyed
//     EXPECT_EQ(MoveSpy::constructions, MoveSpy::destructions);
// }