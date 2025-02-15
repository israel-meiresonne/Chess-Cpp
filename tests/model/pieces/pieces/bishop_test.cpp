#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(BishopTest, DefaultConstructor) {
    Pieces::Bishop bishop;
    EXPECT_EQ(typeid(bishop.position()), typeid(Position));
    EXPECT_EQ(bishop.nMoves(), 0);
}

TEST(BishopTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Bishop bishop(pos);
    EXPECT_EQ(bishop.position(), pos);
    EXPECT_EQ(bishop.nMoves(), 0);
}

TEST(BishopTest, Moves) {
    Pieces::Bishop bishop;
    auto moves = bishop.moves(3, 3);
    EXPECT_TRUE(moves.empty());
}
