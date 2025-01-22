#include "core/span.h"

#include <gtest/gtest.h>

using namespace core;

TEST(Span, Initialize) {
    const char text[] = "hello world";
    auto a = span<const char>{text, 5};
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.data(), text);
    ASSERT_FALSE(a.empty());
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'e');
    ASSERT_EQ(a[2], 'l');
    ASSERT_EQ(a[3], 'l');
    ASSERT_EQ(a[4], 'o');

    auto b = span<int>{};
    ASSERT_TRUE(b.empty());
}

TEST(Span, Subspan) {
    const char text[] = "hello world";
    auto a = span<const char>{text, sizeof(text) - 1};

    auto b = a.subspan(0, 5);
    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b[0], 'h');
    ASSERT_EQ(b[1], 'e');
    ASSERT_EQ(b[2], 'l');
    ASSERT_EQ(b[3], 'l');
    ASSERT_EQ(b[4], 'o');

    auto c = a.subspan(6, 5);
    ASSERT_EQ(c.size(), 5);
    ASSERT_EQ(c[0], 'w');
    ASSERT_EQ(c[1], 'o');
    ASSERT_EQ(c[2], 'r');
    ASSERT_EQ(c[3], 'l');
    ASSERT_EQ(c[4], 'd');

    auto d = c.subspan(1, 2);
    ASSERT_EQ(d.size(), 2);
    ASSERT_EQ(d[0], 'o');
    ASSERT_EQ(d[1], 'r');
}

TEST(Span, Iterator) {
    const char text[] = "abc";
    auto a = span<const char>{text, sizeof(text) - 1};
    const auto* it = a.begin();
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 'a');
    ++it;
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 'b');
    ++it;
    ASSERT_NE(it, a.end());
    ASSERT_EQ(*it, 'c');
    ++it;
    ASSERT_EQ(it, a.end());
}
