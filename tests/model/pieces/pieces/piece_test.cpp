#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/position/position.hpp>

class TestPiece1 : public Pieces::Piece {
  public:
    TestPiece1()
        : Piece() {}
    TestPiece1(const Position &position)
        : Piece(position) {}

    std::unordered_set<Pieces::Move> moves(int, int) override { return {}; }
};

class TestPiece2 : public TestPiece1 {
  public:
    TestPiece2(const Position &position)
        : TestPiece1(position) {}
};

TEST(PieceTest, DefaultConstructor) {
    TestPiece1 piece;
    EXPECT_EQ(typeid(piece.position()), typeid(Position));
    EXPECT_EQ(piece.nMoves(), 0);
}

TEST(PieceTest, PositionConstructor) {
    Position pos(2, 3);
    TestPiece1 piece(pos);
    EXPECT_EQ(piece.position(), pos);
    EXPECT_EQ(piece.nMoves(), 0);
}

TEST(PieceTest, MoveIncrementsMoveCount) {
    Position pos1(1, 1);
    Position pos2(2, 2);
    Position pos3(3, 3);
    TestPiece1 piece(pos1);

    piece.move(pos2);
    EXPECT_EQ(piece.position(), pos2);
    EXPECT_EQ(piece.nMoves(), 1);

    piece.move(pos3);
    EXPECT_EQ(piece.position(), pos3);
    EXPECT_EQ(piece.nMoves(), 2);
}

TEST(PieceTest, EqualityOperator) {
    TestPiece1 piece1(Position(1, 1));
    TestPiece1 piece2(Position(1, 1));
    TestPiece1 piece3(Position(2, 2));
    TestPiece2 piece4(Position(2, 2));

    EXPECT_TRUE(piece1 == piece2);
    EXPECT_FALSE(piece1 == piece3);
    EXPECT_FALSE(piece3 == piece4);
}

TEST(PieceTest, Hash) {
    TestPiece1 piece1(Position(1, 1));
    TestPiece1 piece2(Position(1, 1));
    TestPiece1 piece3(Position(2, 2));
    TestPiece2 piece4(Position(2, 2));

    EXPECT_EQ(piece1.hash(), piece2.hash());
    EXPECT_NE(piece1.hash(), piece3.hash());
    EXPECT_NE(piece3.hash(), piece4.hash());
}

TEST(PieceTest, IsInBounds_ValidPositions) {
    // Top-left corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(0, 0), 8, 8));
    // Bottom-right corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(7, 7), 8, 8));
    // Middle of the board
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(3, 4), 8, 8));
}

TEST(PieceTest, IsInBounds_InvalidPositions) {
    // Out of bounds (negative row)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(-1, 0), 8, 8));
    // Out of bounds (negative column)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(0, -1), 8, 8));
    // Out of bounds (exceeding limits)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(8, 8), 8, 8));
    // Row exceeds board size
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(10, 5), 8, 8));
    // Column exceeds board size
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(3, 9), 8, 8));
}
