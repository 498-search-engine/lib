#include "core/span.h"

#include <gtest/gtest.h>

using namespace core;

TEST(Span, Initialize) {
    const char text[] = "hello world";
    auto a = Span<const char>{text, 5};
    ASSERT_EQ(a.Size(), 5);
    ASSERT_EQ(a.Data(), text);
    ASSERT_FALSE(a.Empty());
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'e');
    ASSERT_EQ(a[2], 'l');
    ASSERT_EQ(a[3], 'l');
    ASSERT_EQ(a[4], 'o');

    auto b = Span<int>{};
    ASSERT_TRUE(b.Empty());
}

TEST(Span, Subspan) {
    const char text[] = "hello world";
    auto a = Span<const char>{text, sizeof(text) - 1};

    auto b = a.Subspan(0, 5);
    ASSERT_EQ(b.Size(), 5);
    ASSERT_EQ(b[0], 'h');
    ASSERT_EQ(b[1], 'e');
    ASSERT_EQ(b[2], 'l');
    ASSERT_EQ(b[3], 'l');
    ASSERT_EQ(b[4], 'o');

    auto c = a.Subspan(6, 5);
    ASSERT_EQ(c.Size(), 5);
    ASSERT_EQ(c[0], 'w');
    ASSERT_EQ(c[1], 'o');
    ASSERT_EQ(c[2], 'r');
    ASSERT_EQ(c[3], 'l');
    ASSERT_EQ(c[4], 'd');

    auto d = c.Subspan(1, 2);
    ASSERT_EQ(d.Size(), 2);
    ASSERT_EQ(d[0], 'o');
    ASSERT_EQ(d[1], 'r');
}

TEST(Span, Iterator) {
    const char text[] = "abc";
    auto a = Span<const char>{text, sizeof(text) - 1};
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
