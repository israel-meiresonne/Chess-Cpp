#include <gtest/gtest.h>

#include <model/player/player.hpp>

// Test Default Constructor
TEST(PlayerTest, DefaultConstructor) {
    Player player;
    EXPECT_EQ(player.name(), "");
}

// Test Parameterized Constructor
TEST(PlayerTest, ParameterizedConstructor) {
    Player player("Alice");
    EXPECT_EQ(player.name(), "Alice");
}

// Test Equality Operator
TEST(PlayerTest, EqualityOperator) {
    Player player1("Alice");
    Player player2("Alice");
    Player player3("Bob");

    EXPECT_TRUE(player1 == player2);
    EXPECT_FALSE(player1 == player3);
}
