#include "../text.h"
#include <gtest/gtest.h>

using namespace mLab;

TEST(Input_test, Test_int_str) {
    std::string str1 = "123";
    int int1 = from_str_to_int(str1);
    EXPECT_EQ(int1, 123);
    std::string str2 = "Hello";
    int int2 = from_str_to_int(str2);
    EXPECT_EQ(int2, -1);
}

