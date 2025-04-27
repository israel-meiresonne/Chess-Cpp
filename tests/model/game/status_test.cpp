#include <gtest/gtest.h>

#include <model/game/game.hpp>

TEST(StatusTest, StringConversion) {
    EXPECT_EQ(std::string(Game::Status::NOT_STARTED), "NOT_STARTED");
    EXPECT_EQ(std::string(Game::Status::IN_PROGRESS), "IN_PROGRESS");
    EXPECT_EQ(std::string(Game::Status::ENDED_CHECKMATE), "ENDED_CHECKMATE");
    EXPECT_EQ(std::string(Game::Status::ENDED_STALEMATE), "ENDED_STALEMATE");
    EXPECT_EQ(std::string(Game::Status::UNDEFINED), "UNDEFINED");
}

TEST(StatusTest, EqualityOperator) {
    EXPECT_TRUE(Game::Status::NOT_STARTED == Game::Status::NOT_STARTED);
    EXPECT_FALSE(Game::Status::NOT_STARTED == Game::Status::IN_PROGRESS);
}

TEST(StatusTest, InequalityOperator) {
    EXPECT_TRUE(Game::Status::IN_PROGRESS != Game::Status::ENDED_STALEMATE);
    EXPECT_FALSE(Game::Status::IN_PROGRESS != Game::Status::IN_PROGRESS);
}
