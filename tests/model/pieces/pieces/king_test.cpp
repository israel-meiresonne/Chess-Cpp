#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(KingTest, DefaultConstructor) {
    Pieces::King king;
    EXPECT_EQ(typeid(king.position()), typeid(Position));
    EXPECT_EQ(king.nMoves(), 0);
}

TEST(KingTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::King king(pos);
    EXPECT_EQ(king.position(), pos);
    EXPECT_EQ(king.nMoves(), 0);
}

TEST(KingTest, MovesReturnsEmptySet) {
    Pieces::King king;
    auto moves = king.moves(4, 4);
    EXPECT_TRUE(moves.empty());
}
