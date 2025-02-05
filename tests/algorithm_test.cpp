#include "core/algorithm.h"

#include <type_traits>
#include <gtest/gtest.h>
#include <vector>

using std::vector;

TEST(AlgorithmTests, ForEach_Basic) {
    vector<int> v = {1,2,3};
    int sum = 0;
    core::for_each(v.begin(), v.end(), [&](auto i){ sum += i;});
    EXPECT_EQ(sum, 6);
}

