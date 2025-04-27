#include "core/map.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using core::Map;

TEST(MapTest, EmptyAndSize) {
    Map<int,std::string> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(MapTest, InsertAndFind) {
    Map<int,std::string> m;
    auto [it1, inserted1] = m.insert(1, "one");
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(it1->first, 1);
    EXPECT_EQ(it1->second, "one");

    // duplicate insert
    auto [it2, inserted2] = m.insert(1, "uno");
    EXPECT_FALSE(inserted2);
    EXPECT_EQ(it2->first, 1);
    EXPECT_EQ(it2->second, "one");  // original value unchanged
    EXPECT_EQ(m.size(), 1u);

    // find existing and missing
    auto found = m.find(1);
    ASSERT_NE(found, m.end());
    EXPECT_EQ(found->second, "one");
    EXPECT_EQ(m.find(2), m.end());
}

TEST(MapTest, OperatorBracketAndAt) {
    Map<int,std::string> m;
    // operator[] should default-construct
    m[2];  
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m[2], "");  

    // assign via []
    m[2] = "two";
    EXPECT_EQ(m[2], "two");

    // at() for existing
    EXPECT_EQ(m.at(2), "two");

    // at() for missing throws
    EXPECT_THROW(m.at(3), std::out_of_range);
}

TEST(MapTest, ContainsAndCount) {
    Map<char,int> m;
    m.insert('a', 10);
    EXPECT_TRUE(m.contains('a'));
    EXPECT_EQ(m.count('a'), 1u);
    EXPECT_FALSE(m.contains('b'));
    EXPECT_EQ(m.count('b'), 0u);
}

TEST(MapTest, IterationOrder) {
    Map<int,int> m;
    m.insert(3, 30);
    m.insert(1, 10);
    m.insert(2, 20);

    std::vector<int> keys;
    for (auto it = m.begin(); it != m.end(); ++it) {
        keys.push_back(it->first);
    }
    EXPECT_EQ(keys, (std::vector<int>{1,2,3}));
}

TEST(MapTest, LowerUpperBoundAndEqualRange) {
    Map<int,int> m;
    for (int i = 1; i <= 5; ++i) {
        m.insert(i, i*100);
    }
    auto lb = m.lower_bound(3);
    ASSERT_NE(lb, m.end());
    EXPECT_EQ(lb->first, 3);

    auto ub = m.upper_bound(3);
    ASSERT_NE(ub, m.end());
    EXPECT_EQ(ub->first, 4);

    auto [r1, r2] = m.equal_range(3);
    EXPECT_EQ(r1->first, 3);
    EXPECT_EQ(r2->first, 4);
}

TEST(MapTest, CopyConstructorDeepCopy) {
    Map<std::string,int> m1;
    m1.insert("x", 42);
    m1.insert("y", 84);

    Map<std::string,int> m2(m1);
    EXPECT_EQ(m2.size(), m1.size());
    EXPECT_EQ(m2["x"], 42);
    EXPECT_EQ(m2["y"], 84);

    // mutate the copy, original must stay the same
    m2["x"] = 100;
    EXPECT_EQ(m2["x"], 100);
    EXPECT_EQ(m1["x"], 42);
}

TEST(MapTest, CustomCompareDescending) {
    Map<int,int,std::greater<>> m(std::greater<>{});
    m.insert(1,10);
    m.insert(2,20);
    m.insert(3,30);

    std::vector<int> keys;
    for (auto& p : m) {
        keys.push_back(p.first);
    }
    EXPECT_EQ(keys, (std::vector<int>{3,2,1}));
}

TEST(MapTest, EmplaceWorks) {
    Map<int,std::string> m;

    // emplace a new element
    auto [it1, inserted1] = m.emplace(4, "four");
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(it1->first, 4);
    EXPECT_EQ(it1->second, "four");
    EXPECT_EQ(m.size(), 1u);

    // emplace duplicate key should not overwrite
    auto [it2, inserted2] = m.emplace(4, "quattro");
    EXPECT_FALSE(inserted2);
    EXPECT_EQ(it2->first, 4);
    EXPECT_EQ(it2->second, "four");
    EXPECT_EQ(m.size(), 1u);
}
