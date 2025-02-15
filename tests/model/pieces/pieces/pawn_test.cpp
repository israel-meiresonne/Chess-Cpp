
#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(PawnTest, DefaultConstructor) {
    Pieces::Pawn pawn;
    EXPECT_EQ(typeid(pawn.position()), typeid(Position));
    EXPECT_EQ(pawn.nMoves(), 0);
}

TEST(PawnTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Pawn pawn(pos);
    EXPECT_EQ(pawn.position(), pos);
    EXPECT_EQ(pawn.nMoves(), 0);
}

TEST(PawnTest, Moves) {
    Pieces::Pawn pawn;
    auto moves = pawn.moves(6, 1);
    EXPECT_TRUE(moves.empty());
}
