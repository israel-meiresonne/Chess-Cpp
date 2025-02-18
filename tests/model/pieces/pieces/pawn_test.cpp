
#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

Pieces::Move newMove(Pieces::Piece *piece, Position initial, Position final,
                     Pieces::Move::Type moveType) {
    Pieces::Action action(piece, initial, final);
    Pieces::Move move(moveType);
    move.add(action);
    return move;
}

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

TEST(PawnTest, NoMoves) {
    Pieces::Pawn pawn;
    auto moves = pawn.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(PawnTest, FirstMoveCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 4;
    int column = 4;
    Pieces::Pawn pawn(Position(row, column));
    Position initial = pawn.position();

    std::vector<Pieces::Move> expectedMoves = {
        newMove(&pawn, initial, Position(row + 1, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 2, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 1, column - 1), Pieces::Move::Type::CAPTURE),
        newMove(&pawn, initial, Position(row + 1, column + 1), Pieces::Move::Type::CAPTURE)};

    auto moves = pawn.moves(nRow, nColumn);
    EXPECT_EQ(moves.size(), expectedMoves.size());

    for (const auto &move : expectedMoves) {
        EXPECT_TRUE(moves.count(move));
    }
}

TEST(PawnTest, FirstMoveTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Pawn pawn(Position(row, column));

    Position initial = pawn.position();
    Position outsideCornerTop(pawn.position().row() + 1, pawn.position().column());
    Position outsideCornerLeft(pawn.position().row(), pawn.position().column() - 1);

    auto moves = pawn.moves(nRow, nColumn);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerTop, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerLeft, nRow, nColumn));

    EXPECT_EQ(moves.size(), 0);
}

TEST(PawnTest, FirstMoveBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Pawn pawn(Position(row, column));

    Position initial = pawn.position();
    Position outsideCornerBottom(pawn.position().row() - 1, pawn.position().column());
    Position outsideCornerRight(pawn.position().row(), pawn.position().column() + 1);

    std::vector<Pieces::Move> expectedMoves = {
        newMove(&pawn, initial, Position(row + 1, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 2, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 1, column - 1), Pieces::Move::Type::CAPTURE)};

    auto moves = pawn.moves(nRow, nColumn);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerBottom, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerRight, nRow, nColumn));

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &move : expectedMoves) {
        EXPECT_TRUE(moves.count(move));
    }
}

TEST(PawnTest, NotFirstMoveCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 4;
    int column = 4;
    Pieces::Pawn pawn(Position(row, column));
    Position initial = pawn.position();

    std::vector<Pieces::Move> expectedMoves = {
        newMove(&pawn, initial, Position(row + 1, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 1, column - 1), Pieces::Move::Type::CAPTURE),
        newMove(&pawn, initial, Position(row + 1, column + 1), Pieces::Move::Type::CAPTURE)};

    pawn.move(pawn.position());
    auto moves = pawn.moves(nRow, nColumn);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &move : expectedMoves) {
        EXPECT_TRUE(moves.count(move));
    }
}

TEST(PawnTest, NotFirstMoveTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Pawn pawn(Position(row, column));

    Position initial = pawn.position();
    Position outsideCornerTop(pawn.position().row() + 1, pawn.position().column());
    Position outsideCornerLeft(pawn.position().row(), pawn.position().column() - 1);

    pawn.move(pawn.position());
    auto moves = pawn.moves(nRow, nColumn);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerTop, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerLeft, nRow, nColumn));

    EXPECT_EQ(moves.size(), 0);
}

TEST(PawnTest, NotFirstMoveBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Pawn pawn(Position(row, column));

    Position initial = pawn.position();
    Position outsideCornerBottom(pawn.position().row() - 1, pawn.position().column());
    Position outsideCornerRight(pawn.position().row(), pawn.position().column() + 1);

    std::vector<Pieces::Move> expectedMoves = {
        newMove(&pawn, initial, Position(row + 1, column), Pieces::Move::Type::DISPLACEMENT),
        newMove(&pawn, initial, Position(row + 1, column - 1), Pieces::Move::Type::CAPTURE)};

    pawn.move(pawn.position());
    auto moves = pawn.moves(nRow, nColumn);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerBottom, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerRight, nRow, nColumn));

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &move : expectedMoves) {
        EXPECT_TRUE(moves.count(move));
    }
}
