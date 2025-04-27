#include "gtest/gtest.h"
#include "core/string.h"

using namespace core;

TEST(StringConversionTest, Stoi) {
    String s = "12345";
    EXPECT_EQ(stoi(s), 12345);

    String s2 = "-987";
    EXPECT_EQ(stoi(s2), -987);
}

TEST(StringConversionTest, Stol) {
    String s = "1234567890";
    EXPECT_EQ(stol(s), 1234567890L);

    String s2 = "-1234567890";
    EXPECT_EQ(stol(s2), -1234567890L);
}

TEST(StringConversionTest, Stoul) {
    String s = "4294967295"; // max uint32_t
    EXPECT_EQ(stoul(s), 4294967295UL);

    // No negative test because unsigned long can't parse negative properly.
}

TEST(StringConversionTest, Stod) {
    String s = "3.14159";
    EXPECT_DOUBLE_EQ(stod(s), 3.14159);

    String s2 = "-2.71828";
    EXPECT_DOUBLE_EQ(stod(s2), -2.71828);
}
