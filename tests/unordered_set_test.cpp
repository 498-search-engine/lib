#include "core/unordered_set.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <algorithm>

using core::UnorderedSet;

TEST(UnorderedSetTest, DefaultConstructor) {
    UnorderedSet<int> s;
    EXPECT_EQ(s.Size(), 0);
    EXPECT_TRUE(s.Empty());
}

TEST(UnorderedSetTest, InsertAndContains) {
    UnorderedSet<std::string> s;
    EXPECT_TRUE(s.insert("apple"));
    EXPECT_TRUE(s.insert("banana"));
    EXPECT_FALSE(s.insert("apple"));  // duplicate insert returns false

    EXPECT_TRUE(s.contains("apple"));
    EXPECT_TRUE(s.contains("banana"));
    EXPECT_FALSE(s.contains("cherry"));
}

TEST(UnorderedSetTest, EraseElement) {
    UnorderedSet<int> s;
    s.insert(1);
    s.insert(2);

    EXPECT_TRUE(s.contains(2));
    EXPECT_TRUE(s.Erase(2));
    EXPECT_FALSE(s.contains(2));
    EXPECT_FALSE(s.Erase(2));
}

TEST(UnorderedSetTest, ClearSet) {
    UnorderedSet<int> s;
    s.insert(10);
    s.insert(20);
    s.insert(30);

    EXPECT_EQ(s.Size(), 3);
    s.clear();
    EXPECT_EQ(s.Size(), 0);
    EXPECT_TRUE(s.Empty());
    EXPECT_FALSE(s.contains(10));
}

TEST(UnorderedSetTest, RangeBasedIteration) {
    UnorderedSet<int> s;
    s.insert(5);
    s.insert(10);
    s.insert(15);

    std::vector<int> found;
    for (const auto& x : s) {
        found.push_back(x);
    }

    std::sort(found.begin(), found.end());

    std::vector<int> expected = {5, 10, 15};
    EXPECT_EQ(found, expected);
}

TEST(UnorderedSetTest, MoveConstructor) {
    UnorderedSet<std::string> s1;
    s1.insert("x");
    s1.insert("y");

    UnorderedSet<std::string> s2(std::move(s1));

    EXPECT_TRUE(s2.contains("x"));
    EXPECT_TRUE(s2.contains("y"));
    EXPECT_TRUE(s1.Empty());
}

TEST(UnorderedSetTest, MoveAssignment) {
    UnorderedSet<std::string> s1;
    s1.insert("hello");

    UnorderedSet<std::string> s2;
    s2 = std::move(s1);

    EXPECT_TRUE(s2.contains("hello"));
    EXPECT_TRUE(s1.Empty());
}