#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

TEST(DirectionTest, ValidConversions) {
    std::pair<int, int> UP = Pieces::Move::Direction::UP;
    std::pair<int, int> DOWN = Pieces::Move::Direction::DOWN;
    std::pair<int, int> LEFT = Pieces::Move::Direction::LEFT;
    std::pair<int, int> RIGHT = Pieces::Move::Direction::RIGHT;
    std::pair<int, int> UP_LEFT = Pieces::Move::Direction::UP_LEFT;
    std::pair<int, int> UP_RIGHT = Pieces::Move::Direction::UP_RIGHT;
    std::pair<int, int> DOWN_LEFT = Pieces::Move::Direction::DOWN_LEFT;
    std::pair<int, int> DOWN_RIGHT = Pieces::Move::Direction::DOWN_RIGHT;
    std::pair<int, int> CUSTOM_INT = Pieces::Move::Direction(5, -3);
    std::pair<int, int> CUSTOM_POSITION = Pieces::Move::Direction(Position(-2, 6), Position(3, -5));
    EXPECT_EQ(UP, std::make_pair(1, 0));
    EXPECT_EQ(DOWN, std::make_pair(-1, 0));
    EXPECT_EQ(LEFT, std::make_pair(0, -1));
    EXPECT_EQ(RIGHT, std::make_pair(0, 1));
    EXPECT_EQ(UP_LEFT, std::make_pair(1, -1));
    EXPECT_EQ(UP_RIGHT, std::make_pair(1, 1));
    EXPECT_EQ(DOWN_LEFT, std::make_pair(-1, -1));
    EXPECT_EQ(DOWN_RIGHT, std::make_pair(-1, 1));
    EXPECT_EQ(CUSTOM_INT, std::make_pair(5, -3));
    EXPECT_EQ(CUSTOM_POSITION, std::make_pair(5, -11));
}

TEST(DirectionTest, UndefinedDirectionThrowsException) {
    Pieces::Move::Direction undefined;
    EXPECT_THROW(({ static_cast<std::pair<int, int>>(undefined); }), std::runtime_error);
}
