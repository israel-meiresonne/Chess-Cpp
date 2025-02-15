
#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(KnightTest, DefaultConstructor) {
    Pieces::Knight knight;
    EXPECT_EQ(typeid(knight.position()), typeid(Position));
    EXPECT_EQ(knight.nMoves(), 0);
}

TEST(KnightTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Knight knight(pos);
    EXPECT_EQ(knight.position(), pos);
    EXPECT_EQ(knight.nMoves(), 0);
}

TEST(KnightTest, Moves) {
    Pieces::Knight knight;
    auto moves = knight.moves(1, 2);
    EXPECT_TRUE(moves.empty());
}
