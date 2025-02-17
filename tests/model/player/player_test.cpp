#include <gtest/gtest.h>

#include <model/player/player.hpp>

TEST(PlayerTest, DefaultConstructor) {
    Player player;
    EXPECT_EQ(player.name(), "");
}

TEST(PlayerTest, ParameterizedConstructor) {
    Player player("Alice");
    EXPECT_EQ(player.name(), "Alice");
}

TEST(PlayerTest, EqualityOperator) {
    Player player1("Alice");
    Player player2("Alice");
    Player player3("Bob");

    EXPECT_TRUE(player1 == player2);
    EXPECT_FALSE(player1 == player3);
}

TEST(PlayerTest, Hash) {
    Player player1("Alice");
    Player player2("Alice");
    Player player3("Bob");

    EXPECT_EQ(player1.hash(), player2.hash());
    EXPECT_NE(player1.hash(), player3.hash());
}
