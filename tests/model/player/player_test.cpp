#include "model/player/player.hpp"

#include <gtest/gtest.h>

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
