#include <gtest/gtest.h>

#include <model/game/status.hpp>

namespace Game {

// Test that the Status enum values exist and are distinct
TEST(StatusTest, EnumValues) {
  EXPECT_EQ(static_cast<int>(Status::NOT_STARTED), 0);
  EXPECT_EQ(static_cast<int>(Status::IN_PROGRESS), 1);
  EXPECT_EQ(static_cast<int>(Status::ENDED_CHECKMATE), 2);
  EXPECT_EQ(static_cast<int>(Status::ENDED_STALEMATE), 3);
}

// Test equality comparison between Status values
TEST(StatusTest, EnumComparison) {
  EXPECT_EQ(Status::NOT_STARTED, Status::NOT_STARTED);
  EXPECT_NE(Status::IN_PROGRESS, Status::ENDED_CHECKMATE);
  EXPECT_NE(Status::ENDED_CHECKMATE, Status::ENDED_STALEMATE);
}

} // namespace Game
