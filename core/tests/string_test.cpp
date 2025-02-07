#include "core/string.h"

#include <gtest/gtest.h>

TEST(STACK_STRINGS, EMPTY_STRING) {
    String emptyString{};
    String emptyString2("");

    EXPECT_EQ(emptyString.Size(), 0);
    EXPECT_EQ(emptyString2.Size(), 0);

    EXPECT_EQ(*emptyString.begin(), '\0');
    EXPECT_EQ(*emptyString.Cstr(), '\0');
    EXPECT_EQ(*emptyString2.begin(), '\0');
    EXPECT_EQ(*emptyString2.Cstr(), '\0');

    EXPECT_EQ(emptyString, emptyString2);
    EXPECT_FALSE(emptyString != emptyString2);
    EXPECT_FALSE(emptyString > emptyString2);
    EXPECT_FALSE(emptyString < emptyString2);
    EXPECT_TRUE(emptyString <= emptyString2);
    EXPECT_TRUE(emptyString >= emptyString2);

    emptyString += emptyString2;

    // String should still be empty!
    EXPECT_EQ(emptyString.Size(), 0);
    EXPECT_EQ(emptyString2.Size(), 0);

    EXPECT_EQ(*emptyString.begin(), '\0');
    EXPECT_EQ(*emptyString.Cstr(), '\0');
    EXPECT_EQ(*emptyString2.begin(), '\0');
    EXPECT_EQ(*emptyString2.Cstr(), '\0');

    EXPECT_EQ(emptyString, emptyString2);
    EXPECT_FALSE(emptyString != emptyString2);
    EXPECT_FALSE(emptyString > emptyString2);
    EXPECT_FALSE(emptyString < emptyString2);
    EXPECT_TRUE(emptyString <= emptyString2);
    EXPECT_TRUE(emptyString >= emptyString2);

    emptyString2 += emptyString;

    EXPECT_EQ(emptyString.Size(), 0);
    EXPECT_EQ(emptyString2.Size(), 0);

    EXPECT_EQ(*emptyString.begin(), '\0');
    EXPECT_EQ(*emptyString.Cstr(), '\0');
    EXPECT_EQ(*emptyString2.begin(), '\0');
    EXPECT_EQ(*emptyString2.Cstr(), '\0');

    EXPECT_EQ(emptyString, emptyString2);
    EXPECT_FALSE(emptyString != emptyString2);
    EXPECT_FALSE(emptyString > emptyString2);
    EXPECT_FALSE(emptyString < emptyString2);
    EXPECT_TRUE(emptyString <= emptyString2);
    EXPECT_TRUE(emptyString >= emptyString2);

    emptyString.PushBack('c');
    emptyString2.PushBack('c');
    emptyString.PopBack();
    emptyString2.PopBack();

    EXPECT_EQ(emptyString.Size(), 0);
    EXPECT_EQ(emptyString2.Size(), 0);

    EXPECT_EQ(*emptyString.begin(), '\0');
    EXPECT_EQ(*emptyString.Cstr(), '\0');
    EXPECT_EQ(*emptyString2.begin(), '\0');
    EXPECT_EQ(*emptyString2.Cstr(), '\0');

    EXPECT_EQ(emptyString, emptyString2);
    EXPECT_FALSE(emptyString != emptyString2);
    EXPECT_FALSE(emptyString > emptyString2);
    EXPECT_FALSE(emptyString < emptyString2);
    EXPECT_TRUE(emptyString <= emptyString2);
    EXPECT_TRUE(emptyString >= emptyString2);
};

TEST(STACK_STRINGS, SMALL_STRING_BASIC) {
    char arr[] = "tomato potato";
    char arr2[] = "tomato potatotomato";
    String string("tomato potato");

    EXPECT_EQ(string.Size(), 13);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr[i]);
        EXPECT_EQ(*(string.begin() + i), arr[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr[i]);
    }

    String string2(arr);
    EXPECT_EQ(string, string2);
    EXPECT_FALSE(string != string2);
    EXPECT_FALSE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_TRUE(string <= string2);
    EXPECT_TRUE(string >= string2);

    string += "tomato";

    ASSERT_EQ(string.Size(), 19);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);

    for (size_t i = 0; i < 6; ++i) {
        string.PopBack();
    }

    EXPECT_EQ(string.Size(), 13);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr[i]);
        EXPECT_EQ(*(string.begin() + i), arr[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr[i]);
    }

    EXPECT_EQ(string, string2);
    EXPECT_FALSE(string != string2);
    EXPECT_FALSE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_TRUE(string <= string2);
    EXPECT_TRUE(string >= string2);

    for (size_t i = 0; i < 6; ++i) {
        string.PushBack(arr[i]);
    }

    ASSERT_EQ(string.Size(), 19);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);

    // Test everything again with grow

    string.Grow();

    ASSERT_TRUE(string.Capacity() > 22);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);
};

TEST(HEAP_STRINGS, HEAP_STRINGS_BASIC) {
    char arr[] = "tomato potato tomato sb";
    char arr2[] = "tomato potato tomato sbtomato";
    String string("tomato potato tomato sb");

    EXPECT_EQ(string.Size(), 23);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr[i]);
        EXPECT_EQ(*(string.begin() + i), arr[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr[i]);
    }

    String string2(arr);
    EXPECT_EQ(string, string2);
    EXPECT_FALSE(string != string2);
    EXPECT_FALSE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_TRUE(string <= string2);
    EXPECT_TRUE(string >= string2);

    string += "tomato";

    ASSERT_EQ(string.Size(), 29);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);

    for (size_t i = 0; i < 6; ++i) {
        string.PopBack();
    }

    EXPECT_EQ(string.Size(), 23);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr[i]);
        EXPECT_EQ(*(string.begin() + i), arr[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr[i]);
    }

    EXPECT_EQ(string, string2);
    EXPECT_FALSE(string != string2);
    EXPECT_FALSE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_TRUE(string <= string2);
    EXPECT_TRUE(string >= string2);

    for (size_t i = 0; i < 6; ++i) {
        string.PushBack(arr[i]);
    }

    ASSERT_EQ(string.Size(), 29);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);

    // Check everything again with resize

    string.Resize(400);

    ASSERT_TRUE(string.Capacity() > 400);
    ASSERT_EQ(string.Size(), 29);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);

    // Grow once again

    size_t nowCapacity = string.Capacity();

    string.Grow();

    ASSERT_TRUE(string.Capacity() > nowCapacity);
    ASSERT_EQ(string.Size(), 29);

    for (size_t i = 0; i < string.Size(); ++i) {
        EXPECT_EQ(string[i], arr2[i]);
        EXPECT_EQ(*(string.begin() + i), arr2[i]);
        EXPECT_EQ(*(string.Cstr() + i), arr2[i]);
    }

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);
};

TEST(MIXED_STRINGS, HEAP_COMPARE_STACK) {
    String string("tomato potato tomato sb");
    String string2("tomato");

    EXPECT_NE(string, string2);
    EXPECT_TRUE(string != string2);
    EXPECT_TRUE(string > string2);
    EXPECT_FALSE(string < string2);
    EXPECT_FALSE(string <= string2);
    EXPECT_TRUE(string >= string2);
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}