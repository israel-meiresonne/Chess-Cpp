#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

using namespace Pieces;

class DirectionTest : public ::testing::Test {
protected:
    Move::Direction UP = Move::Direction::UP;
    Move::Direction DOWN = Move::Direction::DOWN;
    Move::Direction LEFT = Move::Direction::LEFT;
    Move::Direction RIGHT = Move::Direction::RIGHT;
    Move::Direction UP_LEFT = Move::Direction::UP_LEFT;
    Move::Direction UP_RIGHT = Move::Direction::UP_RIGHT;
    Move::Direction DOWN_LEFT = Move::Direction::DOWN_LEFT;
    Move::Direction DOWN_RIGHT = Move::Direction::DOWN_RIGHT;
};

TEST_F(DirectionTest, TestUpDirection) {
    std::vector<int> expected = {1, 0};
    EXPECT_EQ(static_cast<std::vector<int>>(UP), expected);
}

TEST_F(DirectionTest, TestDownDirection) {
    std::vector<int> expected = {-1, 0};
    EXPECT_EQ(static_cast<std::vector<int>>(DOWN), expected);
}

TEST_F(DirectionTest, TestLeftDirection) {
    std::vector<int> expected = {0, -1};
    EXPECT_EQ(static_cast<std::vector<int>>(LEFT), expected);
}

TEST_F(DirectionTest, TestRightDirection) {
    std::vector<int> expected = {0, 1};
    EXPECT_EQ(static_cast<std::vector<int>>(RIGHT), expected);
}

TEST_F(DirectionTest, TestUpLeftDirection) {
    std::vector<int> expected = {1, -1};
    EXPECT_EQ(static_cast<std::vector<int>>(UP_LEFT), expected);
}

TEST_F(DirectionTest, TestUpRightDirection) {
    std::vector<int> expected = {1, 1};
    EXPECT_EQ(static_cast<std::vector<int>>(UP_RIGHT), expected);
}

TEST_F(DirectionTest, TestDownLeftDirection) {
    std::vector<int> expected = {-1, -1};
    EXPECT_EQ(static_cast<std::vector<int>>(DOWN_LEFT), expected);
}

TEST_F(DirectionTest, TestDownRightDirection) {
    std::vector<int> expected = {-1, 1};
    EXPECT_EQ(static_cast<std::vector<int>>(DOWN_RIGHT), expected);
}

TEST_F(DirectionTest, TestUndefinedDirectionThrows) {
    Move::Direction undefined;
    // EXPECT_THROW(static_cast<std::vector<int>>(undefined), std::runtime_error);
    EXPECT_THROW(
      {
          static_cast<void>(static_cast<std::vector<int>>(undefined));
      },
      std::runtime_error);
}
