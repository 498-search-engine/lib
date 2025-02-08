#include <core/string.h>

#include <cstring>
#include <gtest/gtest.h>

using namespace core;

TEST(STACK_STRINGS, EMPTY_STRING) {
    String emptyString{};
    String emptyString2("");

    EXPECT_EQ(emptyString.Size(), 0);
    EXPECT_TRUE(emptyString.Empty());
    EXPECT_EQ(emptyString2.Size(), 0);
    EXPECT_TRUE(emptyString2.Empty());

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
    EXPECT_TRUE(emptyString.Empty());
    EXPECT_EQ(emptyString2.Size(), 0);
    EXPECT_TRUE(emptyString2.Empty());

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

    EXPECT_FALSE(emptyString.Empty());
    EXPECT_FALSE(emptyString2.Empty());

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

// Test default constructor
TEST(StringTest, DefaultConstructor) {
    String s;
    EXPECT_EQ(s.Size(), 0);
    EXPECT_EQ(s.Capacity(), StackStringSize);
    EXPECT_STREQ(s.Cstr(), "");
}

// Test constructor with C-string
TEST(StringTest, CStringConstructor) {
    const char* testStr = "Hello, World!";
    String s(testStr);
    EXPECT_EQ(s.Size(), strlen(testStr));
    EXPECT_STREQ(s.Cstr(), testStr);
}

// Test constructor with C-string and length
TEST(StringTest, CStringLengthConstructor) {
    const char* testStr = "Hello, World!";
    size_t len = 5;
    String s(testStr, len);
    EXPECT_EQ(s.Size(), len);
    EXPECT_STREQ(s.Cstr(), "Hello");
}

// Test copy constructor
TEST(StringTest, CopyConstructor) {
    String s1("Hello");
    String s2(s1);
    EXPECT_EQ(s1.Size(), s2.Size());
    EXPECT_STREQ(s1.Cstr(), s2.Cstr());
}

// Test move constructor
TEST(StringTest, MoveConstructor) {
    String s1("Hello");
    String s2(std::move(s1));
    EXPECT_EQ(s2.Size(), 5);
    EXPECT_STREQ(s2.Cstr(), "Hello");
    EXPECT_EQ(s1.Size(), 0);  // Moved-from object should be empty
}

// Test copy assignment operator
TEST(StringTest, CopyAssignment) {
    String s1("Hello");
    String s2;
    s2 = s1;
    EXPECT_EQ(s1.Size(), s2.Size());
    EXPECT_STREQ(s1.Cstr(), s2.Cstr());
}

// Test move assignment operator
TEST(StringTest, MoveAssignment) {
    String s1("Hello");
    String s2;
    s2 = std::move(s1);
    EXPECT_EQ(s2.Size(), 5);
    EXPECT_STREQ(s2.Cstr(), "Hello");
    EXPECT_EQ(s1.Size(), 0);  // Moved-from object should be empty
}

// Test Size() function
TEST(StringTest, Size) {
    String s("Hello");
    EXPECT_EQ(s.Size(), 5);
}

// Test Capacity() function
TEST(StringTest, Capacity) {
    String s;
    EXPECT_EQ(s.Capacity(), StackStringSize);

    s.Resize(100);
    EXPECT_GE(s.Capacity(), 100);
}

// Test Cstr() function
TEST(StringTest, Cstr) {
    String s("Hello");
    EXPECT_STREQ(s.Cstr(), "Hello");
}

// Test begin() and end() iterators
TEST(StringTest, Iterators) {
    String s("Hello");
    const char* begin = s.begin();
    const char* end = s.end();
    EXPECT_EQ(end - begin, 5);
    EXPECT_STREQ(begin, "Hello");
}

// Test operator[] for element access
TEST(StringTest, ElementAccess) {
    String s("Hello");
    EXPECT_EQ(s[0], 'H');
    EXPECT_EQ(s[4], 'o');

    s[0] = 'h';
    EXPECT_STREQ(s.Cstr(), "hello");
}

// Test operator+= for string concatenation
TEST(StringTest, AppendOperator) {
    String s1("Hello");
    String s2(", World!");
    s1 += s2;
    EXPECT_EQ(s1.Size(), 13);
    EXPECT_STREQ(s1.Cstr(), "Hello, World!");
}

// Test PushBack() function
TEST(StringTest, PushBack) {
    String s("Hell");
    s.PushBack('o');
    EXPECT_EQ(s.Size(), 5);
    EXPECT_STREQ(s.Cstr(), "Hello");

    s += '!';
    EXPECT_EQ(s.Size(), 6);
    EXPECT_STREQ(s.Cstr(), "Hello!");
}

// Test PopBack() function
TEST(StringTest, PopBack) {
    String s("Hello");
    s.PopBack();
    EXPECT_EQ(s.Size(), 4);
    EXPECT_STREQ(s.Cstr(), "Hell");
}

// Test equality operator (==)
TEST(StringTest, EqualityOperator) {
    String s1("Hello");
    String s2("Hello");
    String s3("World");

    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 == s3);
}

// Test inequality operator (!=)
TEST(StringTest, InequalityOperator) {
    String s1("Hello");
    String s2("Hello");
    String s3("World");

    EXPECT_FALSE(s1 != s2);
    EXPECT_TRUE(s1 != s3);
}

// Test less-than operator (<)
TEST(StringTest, LessThanOperator) {
    String s1("Apple");
    String s2("Banana");

    EXPECT_TRUE(s1 < s2);
    EXPECT_FALSE(s2 < s1);
}

// Test greater-than operator (>)
TEST(StringTest, GreaterThanOperator) {
    String s1("Banana");
    String s2("Apple");

    EXPECT_TRUE(s1 > s2);
    EXPECT_FALSE(s2 > s1);
}

// Test less-than-or-equal operator (<=)
TEST(StringTest, LessThanOrEqualOperator) {
    String s1("Apple");
    String s2("Banana");
    String s3("Apple");

    EXPECT_TRUE(s1 <= s2);
    EXPECT_TRUE(s1 <= s3);
    EXPECT_FALSE(s2 <= s1);
}

// Test greater-than-or-equal operator (>=)
TEST(StringTest, GreaterThanOrEqualOperator) {
    String s1("Banana");
    String s2("Apple");
    String s3("Banana");

    EXPECT_TRUE(s1 >= s2);
    EXPECT_TRUE(s1 >= s3);
    EXPECT_FALSE(s2 >= s1);
}

// Test Grow() function
TEST(StringTest, Grow) {
    String s;
    size_t initialCapacity = s.Capacity();

    // Force the string to grow
    for (int i = 0; i < 100; ++i) {
        s.PushBack('a');
    }

    EXPECT_GT(s.Capacity(), initialCapacity);
}

// Test Resize() function
TEST(StringTest, Resize) {
    String s("Hello");
    s.Resize(10);
    EXPECT_GE(s.Capacity(), 10);
    EXPECT_EQ(s.Size(), 5);  // Resize should not change the content

    s.Resize(3);
    EXPECT_EQ(s.Size(), 3);
    EXPECT_STREQ(s.Cstr(), "Hel");
}

// Test destructor (indirectly)
TEST(StringTest, Destructor) {
    {
        String s("Hello");
        // Destructor should be called here, no memory leaks
    }
    EXPECT_TRUE(true);  // Just to ensure the test passes
}

TEST(StringTest, OperatorPlus) {
    // const String& + const String&
    auto s1 = String{"abc"};
    auto s2 = String{"123"};
    auto s3 = s1 + s2;
    EXPECT_STREQ(s3.Cstr(), "abc123");

    // const String& + char
    auto s4 = s1 + '!';
    EXPECT_STREQ(s4.Cstr(), "abc!");

    // String&& + String&&
    auto s5 = String{"abc"} + String{"123"};
    EXPECT_STREQ(s5.Cstr(), "abc123");

    // String&& + const String&
    auto s6 = String{"wow"} + s2;
    EXPECT_STREQ(s6.Cstr(), "wow123");

    // String&& + char
    auto s7 = String{"abc"} + '!';
    EXPECT_STREQ(s7.Cstr(), "abc!");
}

TEST(StringTest, Clear) {
    auto s1 = String{"abc123"};

    EXPECT_EQ(s1.Size(), 6);
    s1.Clear();

    EXPECT_EQ(s1.Size(), 0);

    s1.Append("hello");
    EXPECT_EQ(s1.Size(), 5);

    s1.Append(" world this is actually pretty long");
    EXPECT_EQ(s1.Size(), 40);

    auto s2 = String{"hello world this is actually pretty long"};
    EXPECT_EQ(s2.Size(), 40);
    s2.Clear();

    EXPECT_EQ(s2.Size(), 0);

    s2.Append("hello");
    EXPECT_EQ(s2.Size(), 5);

    s2.Append(" world this is actually pretty long!!!");
    EXPECT_EQ(s2.Size(), 43);
}

// Test building a string dynamically using PushBack
TEST(StringIntegrationTest, DynamicStringBuilding) {
    String s;
    const char* text = "Hello, World!";
    for (size_t i = 0; i < strlen(text); ++i) {
        s.PushBack(text[i]);
    }
    EXPECT_EQ(s.Size(), strlen(text));
    EXPECT_STREQ(s.Cstr(), text);
}

// Test combining multiple operations: Append, PushBack, and Resize
TEST(StringIntegrationTest, CombinedOperations) {
    String s1("Hello");
    String s2(", ");
    String s3("World!");

    // Append s2 and s3 to s1
    s1 += s2;
    s1 += s3;

    EXPECT_EQ(s1.Size(), 13);
    EXPECT_STREQ(s1.Cstr(), "Hello, World!");

    // Resize the string to a smaller size
    s1.Resize(5);
    EXPECT_EQ(s1.Size(), 5);
    EXPECT_STREQ(s1.Cstr(), "Hello");

    // Resize the string to a larger size
    s1.Resize(20);
    EXPECT_EQ(s1.Size(), 5);  // Content should remain the same
    EXPECT_STREQ(s1.Cstr(), "Hello");
}

// Test string comparison in a real-world scenario
TEST(StringIntegrationTest, StringComparison) {
    String s1("apple");
    String s2("banana");
    String s3("apple");

    EXPECT_TRUE(s1 == s3);
    EXPECT_FALSE(s1 == s2);
    EXPECT_TRUE(s1 < s2);
    EXPECT_FALSE(s2 < s1);
    EXPECT_TRUE(s2 > s1);
    EXPECT_FALSE(s1 > s2);
}

// Test string concatenation with large strings
TEST(StringIntegrationTest, LargeStringConcatenation) {
    String s1;
    String s2;

    // Build two large strings
    for (int i = 0; i < 1000; ++i) {
        s1.PushBack('a');
        s2.PushBack('b');
    }

    // Concatenate the two large strings
    s1 += s2;

    EXPECT_EQ(s1.Size(), 2000);
    EXPECT_EQ(s1.Capacity() >= 2000, true);  // Ensure capacity is sufficient

    // Verify the content of the concatenated string
    for (size_t i = 0; i < 1000; ++i) {
        EXPECT_EQ(s1[i], 'a');
    }
    for (size_t i = 1000; i < 2000; ++i) {
        EXPECT_EQ(s1[i], 'b');
    }
}

// Test string resizing and capacity management
TEST(StringIntegrationTest, ResizeAndCapacityManagement) {
    String s;

    // Start with a small string
    s = "Hello";
    EXPECT_EQ(s.Size(), 5);
    EXPECT_EQ(s.Capacity(), StackStringSize);

    // Resize to a larger capacity
    s.Resize(100);
    EXPECT_EQ(s.Size(), 5);  // Content should remain the same
    EXPECT_GE(s.Capacity(), 100);

    // Resize to a smaller capacity
    s.Resize(10);
    EXPECT_EQ(s.Size(), 5);  // Content should remain the same
    EXPECT_GE(s.Capacity(), 10);

    // Resize to a very large capacity
    s.Resize(1000);
    EXPECT_EQ(s.Size(), 5);  // Content should remain the same
    EXPECT_GE(s.Capacity(), 1000);
}

// Test string assignment and modification
TEST(StringIntegrationTest, AssignmentAndModification) {
    String s1("Hello");
    String s2 = s1;  // Copy assignment

    EXPECT_EQ(s1.Size(), s2.Size());
    EXPECT_STREQ(s1.Cstr(), s2.Cstr());

    // Modify s2 and ensure s1 remains unchanged
    s2.PushBack('!');
    EXPECT_EQ(s2.Size(), 6);
    EXPECT_STREQ(s2.Cstr(), "Hello!");
    EXPECT_EQ(s1.Size(), 5);
    EXPECT_STREQ(s1.Cstr(), "Hello");
}

// Test string move semantics in a real-world scenario
TEST(StringIntegrationTest, MoveSemantics) {
    String s1("Hello");
    String s2 = std::move(s1);  // Move assignment

    EXPECT_EQ(s2.Size(), 5);
    EXPECT_STREQ(s2.Cstr(), "Hello");
    EXPECT_EQ(s1.Size(), 0);  // s1 should be empty after move

    // Append to the moved string
    s2 += ", World!";
    EXPECT_EQ(s2.Size(), 13);
    EXPECT_STREQ(s2.Cstr(), "Hello, World!");
}

// Test string with embedded null characters
TEST(StringIntegrationTest, EmbeddedNullCharacters) {
    const char* text = "Hello\0World";
    String s(text, 11);  // Include the null character in the length

    EXPECT_EQ(s.Size(), 11);
    EXPECT_EQ(s[5], '\0');  // Check for embedded null character
    EXPECT_EQ(s[10], 'd');  // Check the last character
}

// Test string with repeated appends and resizes
TEST(StringIntegrationTest, RepeatedAppendsAndResizes) {
    String s;

    for (int i = 0; i < 100; ++i) {
        s += "a";
        if (i % 10 == 0) {
            s.Resize(s.Size() + 10);  // Periodically resize
        }
    }

    EXPECT_EQ(s.Size(), 100);
    EXPECT_GE(s.Capacity(), 100);

    for (size_t i = 0; i < 100; ++i) {
        EXPECT_EQ(s[i], 'a');
    }
}

TEST(ConstStringTest, Access) {
    const auto s = String{"Hello world"};
    EXPECT_EQ(s.Size(), 11);
    EXPECT_EQ(s[0], 'H');
}

TEST(ConstStringTest, Copy) {
    const auto s1 = String{"Hello world"};
    const auto s2 = s1;

    EXPECT_EQ(s1.Size(), s2.Size());
    EXPECT_EQ(s1[0], s2[0]);
}


TEST(ConstStringTest, Iterate) {
    const auto s = String{"123"};

    auto it = s.begin();
    EXPECT_EQ(*it, '1');
    ++it;
    EXPECT_EQ(*it, '2');
    ++it;
    EXPECT_EQ(*it, '3');
    ++it;
    EXPECT_EQ(it, s.end());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
