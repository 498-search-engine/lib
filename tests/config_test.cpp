#include "core/config.h"

#include <gtest/gtest.h>

using namespace core;

TEST(Config, Basic) {
    Config config("config_test.conf");
    
    ASSERT_EQ(config.GetString("tomato"), "potato");
    ASSERT_EQ(config.GetString("tomato2"), "potato2");
    ASSERT_EQ(config.GetString("tomato3"), "potato3");
    ASSERT_EQ(config.GetString("tomato4"), "pot");
    ASSERT_EQ(config.GetString("aa"), "ok");
    ASSERT_EQ(config.GetString("aaa"), "34");
    ASSERT_EQ(config.GetString("aaaf"), "53");

    ASSERT_ANY_THROW(config.GetString(""));
    ASSERT_ANY_THROW(config.GetString("non existent key"));
    
    ASSERT_EQ(config.GetString("non_existent_key", "3"), "3");

    ASSERT_EQ(config.GetInt("aaa"), 34);
    ASSERT_EQ(config.GetInt("aaaf"), 53);

    ASSERT_EQ(config.GetInt("non_existent_key", 2), 2);
}