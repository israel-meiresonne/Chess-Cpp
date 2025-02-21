
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

TEST(RookTest, MovesEmpty) {
    Pieces::Rook rook;
    auto moves = rook.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(RookTest, Moves_WhenPieceAtTheCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 3;
    int column = 3;
    Pieces::Rook rook(Position(row, column));
    Position initial = rook.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&rook, initial, Position(row + 1, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 2, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 3, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 4, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 1, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 2, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 3, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&rook, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 2), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 3), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 4), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 2), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 3), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = rook.moves(nRow, nColumn);
    EXPECT_EQ(moves.size(), expectedMoves.size());

    for (const auto &move : moves) {
        for (const auto &action : move.actions()) {
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.initial(), nRow, nColumn));
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.final(), nRow, nColumn));
        }
    }
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}

TEST(RookTest, Moves_WhenPieceAtTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Rook rook(Position(row, column));

    Position initial = rook.position();
    Position outsideCornerTop(rook.position().row() + 1, rook.position().column());
    Position outsideCornerLeft(rook.position().row(), rook.position().column() - 1);

    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&rook, initial, Position(row - 1, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 2, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 3, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 4, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 5, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 6, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row - 7, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&rook, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 2), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 3), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 4), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 5), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 6), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column + 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = rook.moves(nRow, nColumn);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    EXPECT_TRUE(Pieces::Piece::isInBounds(rook.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerTop, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerLeft, nRow, nColumn));

    for (const auto &move : moves) {
        for (const auto &action : move.actions()) {
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.initial(), nRow, nColumn));
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.final(), nRow, nColumn));
        }
    }
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}

TEST(RookTest, Moves_WhenPieceAtBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Rook rook(Position(row, column));

    Position initial = rook.position();
    Position outsideCornerBottom(rook.position().row() - 1, rook.position().column());
    Position outsideCornerRight(rook.position().row(), rook.position().column() + 1);

    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&rook, initial, Position(row + 1, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 2, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 3, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 4, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 5, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 6, column), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row + 7, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&rook, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 2), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 3), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 4), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 5), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 6), moveType),
            Pieces::Move::createMove(&rook, initial, Position(row, column - 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = rook.moves(nRow, nColumn);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    EXPECT_TRUE(Pieces::Piece::isInBounds(rook.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerBottom, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outsideCornerRight, nRow, nColumn));

    for (const auto &move : moves) {
        for (const auto &action : move.actions()) {
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.initial(), nRow, nColumn));
            EXPECT_TRUE(Pieces::Piece::isInBounds(action.final(), nRow, nColumn));
        }
    }
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}
