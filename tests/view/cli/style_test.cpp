#include <gtest/gtest.h>

#include <view/view.hpp>

TEST(StylesTest, StringConversion) {
    EXPECT_EQ(std::string(View::CLI::Style::NORMAL), "\033[0m");
    EXPECT_EQ(std::string(View::CLI::Style::WARNING), "\033[33m");
    EXPECT_EQ(std::string(View::CLI::Style::ERROR), "\033[31m");
    EXPECT_EQ(std::string(View::CLI::Style::SUCCESS), "\033[32m");
}

TEST(StylesTest, EqualityOperator) {
    EXPECT_TRUE(View::CLI::Style::NORMAL == View::CLI::Style::NORMAL);
    EXPECT_FALSE(View::CLI::Style::NORMAL == View::CLI::Style::WARNING);
}

TEST(StylesTest, InequalityOperator) {
    EXPECT_TRUE(View::CLI::Style::NORMAL != View::CLI::Style::WARNING);
    EXPECT_FALSE(View::CLI::Style::NORMAL != View::CLI::Style::NORMAL);
}
