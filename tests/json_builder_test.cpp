#include "core/JSONBuilder.h"
#include "gtest/gtest.h"
#include <iostream>


using namespace core;

TEST(JSONBuilderTest, ToJsonWorksWithPairs) {
    JSONBuilder jb("");
    jb.to_json("name", "John", "age", 30);
    std::cout << "to_json worked\n";
    std::string result = jb.dump();
    std::cout << result << std::endl;
    EXPECT_NE(result.find("\"name\": \"John"), std::string::npos);
    EXPECT_NE(result.find("\"age\": \"30\""), std::string::npos);
}

TEST(JSONBuilderTest, DumpOutputsCorrectFormat) {
    JSONBuilder jb("");
    jb.to_json("key", "value");

    std::string result = jb.dump();
    std::cout << result << std::endl;
    EXPECT_TRUE(result.find("{") != std::string::npos);
    EXPECT_TRUE(result.find("}") != std::string::npos);
    EXPECT_TRUE(result.find("\"key\": \"value\"") != std::string::npos);
}

TEST(JSONBuilderTest, LoadReplacesContent) {
    JSONBuilder jb("");
    jb.to_json("x", "1");
    std::string jsonString = "{ \"a\": \"2\", \"b\": \"3\" }";
    jb.load(jsonString);

    std::string result = jb.dump();
    std::cout << result << "\n";
    EXPECT_TRUE(result.find("\"a\": \"2\"") != std::string::npos);
    EXPECT_TRUE(result.find("\"b\": \"3\"") != std::string::npos);
    EXPECT_TRUE(result.find("\"x\": \"1\"") == std::string::npos);  // Old data should be gone
}

TEST(JSONBuilderTest, ConstructorParsesSimpleJSON) {
    std::string input = R"({"one": "1", "two": "2"})";
    JSONBuilder jb(input);

    std::string result = jb.dump();
    std::cout << result << "\n";
    EXPECT_TRUE(result.find("\"one\": \"1\"") != std::string::npos);
    EXPECT_TRUE(result.find("\"two\": \"2\"") != std::string::npos);
}
