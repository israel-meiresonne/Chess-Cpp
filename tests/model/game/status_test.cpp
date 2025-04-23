#include <gtest/gtest.h>

#include <model/game/game.hpp>

// Test that the Status enum values exist and are distinct
TEST(StatusTest, EnumValues) {
    EXPECT_EQ(static_cast<int>(Game::Status::NOT_STARTED), 0);
    EXPECT_EQ(static_cast<int>(Game::Status::IN_PROGRESS), 1);
    EXPECT_EQ(static_cast<int>(Game::Status::ENDED_CHECKMATE), 2);
    EXPECT_EQ(static_cast<int>(Game::Status::ENDED_STALEMATE), 3);
}

// Test equality comparison between Status values
TEST(StatusTest, EnumComparison) {
    EXPECT_EQ(Game::Status::NOT_STARTED, Game::Status::NOT_STARTED);
    EXPECT_NE(Game::Status::IN_PROGRESS, Game::Status::ENDED_CHECKMATE);
    EXPECT_NE(Game::Status::ENDED_CHECKMATE, Game::Status::ENDED_STALEMATE);
}
