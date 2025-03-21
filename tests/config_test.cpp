#include "core/config.h"

#include <gtest/gtest.h>

using namespace core;

TEST(ConfigTest, Basic) {
    Config config("Basic.conf");
    ASSERT_EQ(config.GetString("tomato"), "potato");
    ASSERT_EQ(config.GetString("tomato2"), "potato2");
    ASSERT_EQ(config.GetString("tomato3"), "potaot3");
    ASSERT_EQ(config.GetString("tomato4"), "pot");
    ASSERT_EQ(config.GetString("aa"), "ok");
    ASSERT_EQ(config.GetString("aaa"), "34");
    ASSERT_EQ(config.GetString("aaaf"), "53");

    ASSERT_ANY_THROW(config.GetString(""));
    ASSERT_ANY_THROW(config.GetString("non existent key"));

    ASSERT_ANY_THROW(config.GetInt("tomato"));
    ASSERT_ANY_THROW(config.GetInt("tomato2"));
    ASSERT_ANY_THROW(config.GetInt("tomato3"));
    ASSERT_ANY_THROW(config.GetInt("tomato4"));

    ASSERT_EQ(config.GetString("non_existent_key", "3"), "3");

    ASSERT_EQ(config.GetInt("aaa"), 34);
    ASSERT_EQ(config.GetInt("aaaf"), 53);

    ASSERT_EQ(config.GetInt("non_existent_key", 2), 2);
}

TEST(ConfigTest, ParsesBasicKeyValuePairs) {
    core::Config config("ParsesBasicKeyValuePairs.conf");
    EXPECT_EQ(config.GetString("name"), "John Doe");
    EXPECT_EQ(config.GetInt("age"), 30);
}

TEST(ConfigTest, IgnoresComments) {
    core::Config config("IgnoresComments.conf");
    EXPECT_EQ(config.GetString("key1"), "value1");
    EXPECT_EQ(config.GetString("key2"), "value2");
}

TEST(ConfigTest, TrimsWhitespace) {
    core::Config config("TrimsWhitespace.conf");
    EXPECT_EQ(config.GetString("key"), "value with spaces");
}

TEST(ConfigTest, IgnoresInvalidLines) {
    core::Config config("IgnoresInvalidLines.conf");
    EXPECT_THROW(config.GetString("invalid_line"), std::invalid_argument);
    EXPECT_EQ(config.GetString("key"), "value");
}

TEST(ConfigTest, OverwritesDuplicateKeys) {
    core::Config config("OverwritesDuplicateKeys.conf");
    EXPECT_EQ(config.GetString("key"), "second");
}

TEST(ConfigTest, HandlesInlineValues) {
    core::Config config("HandlesInlineValues.conf");
    EXPECT_EQ(config.GetString("key"), "value#not_a_comment");
}

TEST(ConfigTest, ParsesIntegers) {
    core::Config config("ParsesIntegers.conf");
    EXPECT_EQ(config.GetInt("count"), 42);
    EXPECT_EQ(config.GetInt("count", 100), 42);
}

TEST(ConfigTest, HandlesMissingKeys) {
    core::Config config("HandlesMissingKeys.conf");
    EXPECT_THROW(config.GetString("missing"), std::invalid_argument);
    EXPECT_EQ(config.GetString("missing", "default"), "default");
    EXPECT_EQ(config.GetInt("missing", 99), 99);
}

TEST(ConfigTest, HandlesMultipleColons) {
    core::Config config("HandlesMultipleColons.conf");
    EXPECT_EQ(config.GetString("path"), "/usr/local:/usr/bin");
}

TEST(ConfigTest, HandlesEmptyValue) {
    core::Config config("HandlesEmptyValue.conf");
    EXPECT_EQ(config.GetString("empty_val"), "");
}

TEST(ConfigTest, IgnoresWhitespaceOnlyLines) {
    core::Config config("IgnoresWhitespaceOnlyLines.conf");
    EXPECT_EQ(config.GetString("key"), "value");
}