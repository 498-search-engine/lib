#include "gtest/gtest.h"
#include "core/tuple.h"  

using namespace core; 

TEST(TupleTest, BasicGet) {
    Tuple<int, double, std::string> t(42, 3.14, "mithril");

    EXPECT_EQ(get<0>(t), 42);
    EXPECT_DOUBLE_EQ(get<1>(t), 3.14);
    EXPECT_EQ(get<2>(t), "mithril");
}

TEST(TupleTest, ModifyThroughGet) {
    Tuple<int, double, std::string> t(1, 2.71, "before");

    get<0>(t) = 100;
    get<1>(t) = 1.618;
    get<2>(t) = "after";

    EXPECT_EQ(get<0>(t), 100);
    EXPECT_DOUBLE_EQ(get<1>(t), 1.618);
    EXPECT_EQ(get<2>(t), "after");
}

TEST(TupleTest, ConstAccess) {
    const Tuple<int, char, bool> t(5, 'x', true);

    EXPECT_EQ(get<0>(t), 5);
    EXPECT_EQ(get<1>(t), 'x');
    EXPECT_TRUE(get<2>(t));
}

TEST(TupleTest, LargeTupleGet) {
    Tuple<int, int, int, int, int, int, int, int, int, int> t(0,1,2,3,4,5,6,7,8,9);

    EXPECT_EQ(get<0>(t), 0);
    EXPECT_EQ(get<5>(t), 5);
    EXPECT_EQ(get<9>(t), 9);
}

TEST(TupleTest, NestedTuple) {
    Tuple<int, Tuple<double, std::string>, bool> t(10, Tuple<double, std::string>(3.14, "nested"), true);

    EXPECT_EQ(get<0>(t), 10);
    auto& nested = get<1>(t);
    EXPECT_DOUBLE_EQ(get<0>(nested), 3.14);
    EXPECT_EQ(get<1>(nested), "nested");
    EXPECT_TRUE(get<2>(t));
}
