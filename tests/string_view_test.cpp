#include "core/string_view.h"  // Adjust path as needed
#include <gtest/gtest.h>
#include <string>

using namespace core;
using namespace core::string_view_literals;

// Test fixture for StringView tests
class StringViewTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for tests
    }

    void TearDown() override {
        // Common cleanup for tests
    }

    // Helper method to create a StringView with known content
    StringView CreateTestView() { return StringView("Hello, World!"); }
};

// Test constructors
TEST_F(StringViewTest, Constructors) {
    // Default constructor
    StringView empty;
    EXPECT_EQ(empty.Size(), 0);
    EXPECT_EQ(empty.Data(), nullptr);
    EXPECT_TRUE(empty.Empty());

    // Constructor with C-string and length
    const char* testStr = "Test String";
    size_t testLen = 11;
    StringView sv1(testStr, testLen);
    EXPECT_EQ(sv1.Size(), testLen);
    EXPECT_EQ(sv1.Data(), testStr);
    EXPECT_FALSE(sv1.Empty());

    // Constructor with null-terminated C-string
    StringView sv2(testStr);
    EXPECT_EQ(sv2.Size(), testLen);
    EXPECT_EQ(sv2.Data(), testStr);
    EXPECT_FALSE(sv2.Empty());

    // nullptr constructor should be deleted, can't test directly
}

// Test iterators
TEST_F(StringViewTest, Iterators) {
    StringView sv = CreateTestView();

    // Test begin/end
    EXPECT_EQ(*sv.begin(), 'H');
    EXPECT_EQ(*(sv.end() - 1), '!');

    // Test cbegin/cend
    EXPECT_EQ(*sv.cbegin(), 'H');
    EXPECT_EQ(*(sv.cend() - 1), '!');

    // Iterate through the view
    std::string result;
    for (auto it = sv.begin(); it != sv.end(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, "Hello, World!");

    // Range-based for loop
    result.clear();
    for (char c : sv) {
        result.push_back(c);
    }
    EXPECT_EQ(result, "Hello, World!");
}

// Test element access
TEST_F(StringViewTest, ElementAccess) {
    StringView sv = CreateTestView();

    // Test operator[]
    EXPECT_EQ(sv[0], 'H');
    EXPECT_EQ(sv[7], 'W');
    EXPECT_EQ(sv[12], '!');

    // Test Front() and Back()
    EXPECT_EQ(sv.Front(), 'H');
    EXPECT_EQ(sv.Back(), '!');
}

// Test capacity methods
TEST_F(StringViewTest, Capacity) {
    StringView sv = CreateTestView();

    // Test Size() and Length()
    EXPECT_EQ(sv.Size(), 13);
    EXPECT_EQ(sv.Length(), 13);
    EXPECT_FALSE(sv.Empty());

    // Test Empty() with empty string
    StringView empty;
    EXPECT_TRUE(empty.Empty());
}

// Test modifiers
TEST_F(StringViewTest, Modifiers) {
    StringView sv = CreateTestView();

    // Test RemovePrefix
    sv.RemovePrefix(7);
    EXPECT_EQ(sv.Size(), 6);
    EXPECT_EQ(sv.Front(), 'W');
    EXPECT_EQ(std::string(sv.Data(), sv.Size()), "World!");

    // Test RemoveSuffix
    sv.RemoveSuffix(1);
    EXPECT_EQ(sv.Size(), 5);
    EXPECT_EQ(sv.Back(), 'd');
    EXPECT_EQ(std::string(sv.Data(), sv.Size()), "World");

    // Test extreme cases
    StringView full = CreateTestView();
    StringView copy = full;
    copy.RemovePrefix(full.Size());
    EXPECT_TRUE(copy.Empty());

    copy = full;
    copy.RemoveSuffix(full.Size());
    EXPECT_TRUE(copy.Empty());
}

// Test substrings
TEST_F(StringViewTest, Substrings) {
    StringView sv = CreateTestView();

    // Basic substring
    StringView sub1 = sv.Substr(7, 5);
    EXPECT_EQ(sub1.Size(), 5);
    EXPECT_EQ(std::string(sub1.Data(), sub1.Size()), "World");

    // Substring to the end
    StringView sub2 = sv.Substr(7);
    EXPECT_EQ(sub2.Size(), 6);
    EXPECT_EQ(std::string(sub2.Data(), sub2.Size()), "World!");

    // Empty substring
    StringView sub3 = sv.Substr(sv.Size());
    EXPECT_TRUE(sub3.Empty());

    // Substring with count exceeding the string length
    StringView sub4 = sv.Substr(7, 100);
    EXPECT_EQ(sub4.Size(), 6);
    EXPECT_EQ(std::string(sub4.Data(), sub4.Size()), "World!");
}

// Test comparison methods
TEST_F(StringViewTest, Comparison) {
    StringView sv1("hello");
    StringView sv2("hello");
    StringView sv3("world");
    const char* cHello = "hello";
    const char* cWorld = "world";

    // Compare StringView with StringView
    EXPECT_EQ(sv1.Compare(sv2), 0);
    EXPECT_LT(sv1.Compare(sv3), 0);
    EXPECT_GT(sv3.Compare(sv1), 0);

    // Compare StringView with C-string
    EXPECT_EQ(sv1.Compare(cHello), 0);
    EXPECT_LT(sv1.Compare(cWorld), 0);
    EXPECT_GT(sv3.Compare(cHello), 0);

    // Test operator overloads
    EXPECT_TRUE(sv1 == sv2);
    EXPECT_FALSE(sv1 == sv3);

    EXPECT_TRUE(sv1 < sv3);
    EXPECT_FALSE(sv3 < sv1);

    EXPECT_TRUE(sv1 <= sv2);
    EXPECT_TRUE(sv1 <= sv3);
    EXPECT_FALSE(sv3 <= sv1);

    EXPECT_TRUE(sv3 > sv1);
    EXPECT_FALSE(sv1 > sv3);

    EXPECT_TRUE(sv3 >= sv1);
    EXPECT_TRUE(sv2 >= sv1);
    EXPECT_FALSE(sv1 >= sv3);
}

// Test StartsWith methods
TEST_F(StringViewTest, StartsWith) {
    StringView sv("Hello, World!");

    // Test StartsWith(StringView)
    EXPECT_TRUE(sv.StartsWith(StringView("Hello")));
    EXPECT_FALSE(sv.StartsWith(StringView("World")));

    // Test StartsWith(char)
    EXPECT_TRUE(sv.StartsWith('H'));
    EXPECT_FALSE(sv.StartsWith('W'));

    // Test StartsWith(const char*)
    EXPECT_TRUE(sv.StartsWith("Hello"));
    EXPECT_FALSE(sv.StartsWith("World"));

    // Edge cases
    EXPECT_TRUE(sv.StartsWith(StringView()));
    EXPECT_TRUE(sv.StartsWith(""));
    EXPECT_FALSE(StringView().StartsWith('H'));
}

// Test EndsWith methods
TEST_F(StringViewTest, EndsWith) {
    StringView sv("Hello, World!");

    // Test EndsWith(StringView)
    EXPECT_TRUE(sv.EndsWith(StringView("World!")));
    EXPECT_FALSE(sv.EndsWith(StringView("Hello")));

    // Test EndsWith(char)
    EXPECT_TRUE(sv.EndsWith('!'));
    EXPECT_FALSE(sv.EndsWith('o'));

    // Test EndsWith(const char*)
    EXPECT_TRUE(sv.EndsWith("World!"));
    EXPECT_FALSE(sv.EndsWith("Hello"));

    // Edge cases
    EXPECT_TRUE(sv.EndsWith(StringView()));
    EXPECT_TRUE(sv.EndsWith(""));
    EXPECT_FALSE(StringView().EndsWith('!'));
}

// Test Find methods
TEST_F(StringViewTest, Find) {
    StringView sv("Hello, World! Hello again.");

    // Test Find(StringView, pos)
    EXPECT_EQ(sv.Find(StringView("Hello"), 0), 0);
    EXPECT_EQ(sv.Find(StringView("World"), 0), 7);
    EXPECT_EQ(sv.Find(StringView("Hello"), 1), 14);
    EXPECT_EQ(sv.Find(StringView("NotFound"), 0), StringView::Npos);

    // Test Find(char, pos)
    EXPECT_EQ(sv.Find('H', 0), 0);
    EXPECT_EQ(sv.Find('W', 0), 7);
    EXPECT_EQ(sv.Find('H', 1), 14);
    EXPECT_EQ(sv.Find('Z', 0), StringView::Npos);

    // Test Find(const char*, pos)
    EXPECT_EQ(sv.Find("Hello", 0), 0);
    EXPECT_EQ(sv.Find("World", 0), 7);
    EXPECT_EQ(sv.Find("Hello", 1), 14);
    EXPECT_EQ(sv.Find("NotFound", 0), StringView::Npos);

    // Edge cases
    EXPECT_EQ(sv.Find("", 0), 0);  // Empty string is found at every position

    // Out of bounds position
    EXPECT_EQ(sv.Find("Hello", sv.Size()), StringView::Npos);

    // Find at the very end
    StringView endSv("Test!");
    EXPECT_EQ(endSv.Find('!', 0), 4);
}

// Test string_view_literals
TEST_F(StringViewTest, Literals) {
    using namespace core::string_view_literals;

    auto sv = "Hello, World!"_sv;
    EXPECT_EQ(sv.Size(), 13);
    EXPECT_EQ(std::string(sv.Data(), sv.Size()), "Hello, World!");

    // Test empty literal
    auto empty = ""_sv;
    EXPECT_TRUE(empty.Empty());
}

// Test edge cases
TEST_F(StringViewTest, EdgeCases) {
    // Empty string view
    StringView empty;
    EXPECT_TRUE(empty.Empty());
    EXPECT_EQ(empty.Size(), 0);
    EXPECT_EQ(empty.Length(), 0);

    // Single character string view
    StringView single("a");
    EXPECT_EQ(single.Size(), 1);
    EXPECT_EQ(single.Front(), 'a');
    EXPECT_EQ(single.Back(), 'a');

    // String view with embedded null characters
    StringView withNull("Hello\0World", 11);
    EXPECT_EQ(withNull.Size(), 11);
    EXPECT_NE(std::string(withNull.Data()), "HelloWorld");  // Will stop at null char in string constructor
    EXPECT_EQ(std::string(withNull.Data(), withNull.Size()), std::string("Hello\0World", 11));
}
