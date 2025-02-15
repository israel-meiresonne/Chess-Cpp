
#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(RookTest, DefaultConstructor) {
    Pieces::Rook rook;
    EXPECT_EQ(typeid(rook.position()), typeid(Position));
    EXPECT_EQ(rook.nMoves(), 0);
}

TEST(RookTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Rook rook(pos);
    EXPECT_EQ(rook.position(), pos);
    EXPECT_EQ(rook.nMoves(), 0);
}

TEST(RookTest, Moves) {
    Pieces::Rook rook;
    auto moves = rook.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}
