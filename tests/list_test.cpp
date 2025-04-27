#include "core/list.h"
#include <gtest/gtest.h>
#include <string>

using namespace core;

TEST(ListTest, DefaultConstructor) {
    List<int> l;
    EXPECT_EQ(l.size(), 0);
    EXPECT_TRUE(l.empty());
}

TEST(ListTest, PushBack) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    EXPECT_EQ(l.size(), 3);

    auto it = l.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(it, l.end());
}

TEST(ListTest, PushFront) {
    List<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);

    EXPECT_EQ(l.size(), 3);

    auto it = l.begin();
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(it, l.end());
}

TEST(ListTest, PopBack) {
    List<int> l;
    l.push_back(5);
    l.push_back(6);
    l.pop_back();

    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(*l.begin(), 5);
}

TEST(ListTest, PopFront) {
    List<int> l;
    l.push_back(7);
    l.push_back(8);
    l.pop_front();

    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(*l.begin(), 8);
}

TEST(ListTest, InsertMiddle) {
    List<int> l;
    l.push_back(1);
    l.push_back(3);

    auto it = l.begin();
    ++it; // points to 3
    l.insert(it, 2);

    EXPECT_EQ(l.size(), 3);

    it = l.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
}

TEST(ListTest, EraseElement) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    auto it = l.begin();
    ++it; // points to 2
    l.erase(it);

    EXPECT_EQ(l.size(), 2);

    it = l.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(it, l.end());
}

TEST(ListTest, CopyConstructor) {
    List<std::string> l1;
    l1.push_back("a");
    l1.push_back("b");

    List<std::string> l2(l1);
    EXPECT_EQ(l2.size(), 2);

    auto it = l2.begin();
    EXPECT_EQ(*it++, "a");
    EXPECT_EQ(*it++, "b");
}

TEST(ListTest, CopyAssignment) {
    List<std::string> l1;
    l1.push_back("x");
    l1.push_back("y");

    List<std::string> l2;
    l2 = l1;

    EXPECT_EQ(l2.size(), 2);
    auto it = l2.begin();
    EXPECT_EQ(*it++, "x");
    EXPECT_EQ(*it++, "y");
}

TEST(ListTest, MoveConstructor) {
    List<int> l1;
    l1.push_back(100);
    l1.push_back(200);

    List<int> l2(std::move(l1));
    EXPECT_EQ(l2.size(), 2);
    EXPECT_TRUE(l1.empty());

    auto it = l2.begin();
    EXPECT_EQ(*it++, 100);
    EXPECT_EQ(*it++, 200);
}

TEST(ListTest, MoveAssignment) {
    List<int> l1;
    l1.push_back(7);
    l1.push_back(8);

    List<int> l2;
    l2 = std::move(l1);

    EXPECT_EQ(l2.size(), 2);
    EXPECT_TRUE(l1.empty());

    auto it = l2.begin();
    EXPECT_EQ(*it++, 7);
    EXPECT_EQ(*it++, 8);
}

TEST(ListTest, EmptyErase) {
    List<int> l;
    auto it = l.begin();
    EXPECT_EQ(l.erase(it), l.end());
}

TEST(ListTest, InsertIntoEmptyList) {
    List<int> l;
    auto it = l.begin();
    l.insert(it, 42);

    EXPECT_EQ(l.size(), 1);
    EXPECT_EQ(*l.begin(), 42);
}

TEST(ListTest, RangeBasedIteration) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    auto curr = 1;
    for (auto& i : l){
        EXPECT_EQ(i, curr++);
    }
}
