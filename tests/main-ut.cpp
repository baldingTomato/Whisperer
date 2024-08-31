#include <gtest/gtest.h>

class BasicTest : public testing::Test {};

TEST_F(BasicTest, GTestWorks) {
    EXPECT_EQ(0, 0);
}
