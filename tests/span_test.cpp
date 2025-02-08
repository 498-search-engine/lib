#include "core/span.h"

#include <gtest/gtest.h>

using namespace core;

TEST(Span, Initialize) {
    const char text[] = "hello world";
    auto a = Span<const char>{text, 5};
    ASSERT_EQ(a.Size(), 5);
    ASSERT_EQ(a.Data(), text);
    ASSERT_FALSE(a.Empty());
    EXPECT_EQ(a[0], 'h');
    EXPECT_EQ(a[1], 'e');
    EXPECT_EQ(a[2], 'l');
    EXPECT_EQ(a[3], 'l');
    EXPECT_EQ(a[4], 'o');

    auto b = Span<int>{};
    ASSERT_TRUE(b.Empty());

    auto c = Span<const char>{"hello"};
    ASSERT_EQ(c.Size(), 6);  // includes \0 terminator
    EXPECT_EQ(c[0], 'h');
    EXPECT_EQ(c[1], 'e');
    EXPECT_EQ(c[2], 'l');
    EXPECT_EQ(c[3], 'l');
    EXPECT_EQ(c[4], 'o');
    EXPECT_EQ(c[5], '\0');
}

TEST(Span, Subspan) {
    const char text[] = "hello world";
    auto a = Span<const char>{text, sizeof(text) - 1};

    auto b = a.Subspan(0, 5);
    ASSERT_EQ(b.Size(), 5);
    EXPECT_EQ(b[0], 'h');
    EXPECT_EQ(b[1], 'e');
    EXPECT_EQ(b[2], 'l');
    EXPECT_EQ(b[3], 'l');
    EXPECT_EQ(b[4], 'o');

    auto c = a.Subspan(6, 5);
    ASSERT_EQ(c.Size(), 5);
    EXPECT_EQ(c[0], 'w');
    EXPECT_EQ(c[1], 'o');
    EXPECT_EQ(c[2], 'r');
    EXPECT_EQ(c[3], 'l');
    EXPECT_EQ(c[4], 'd');

    auto d = c.Subspan(1, 2);
    ASSERT_EQ(d.Size(), 2);
    EXPECT_EQ(d[0], 'o');
    EXPECT_EQ(d[1], 'r');
}

TEST(Span, Iterator) {
    const char text[] = "abc";
    auto a = Span<const char>{text, sizeof(text) - 1};
    const auto* it = a.begin();
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 'a');
    ++it;
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 'b');
    ++it;
    ASSERT_NE(it, a.end());
    EXPECT_EQ(*it, 'c');
    ++it;
    ASSERT_EQ(it, a.end());
}
