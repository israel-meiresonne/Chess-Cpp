
#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(QueenTest, DefaultConstructor) {
    Pieces::Queen queen;
    EXPECT_EQ(typeid(queen.position()), typeid(Position));
    EXPECT_EQ(queen.nMoves(), 0);
}

TEST(QueenTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Queen queen(pos);
    EXPECT_EQ(queen.position(), pos);
    EXPECT_EQ(queen.nMoves(), 0);
}

TEST(QueenTest, Moves) {
    Pieces::Queen queen;
    auto moves = queen.moves(3, 3);
    EXPECT_TRUE(moves.empty());
}
