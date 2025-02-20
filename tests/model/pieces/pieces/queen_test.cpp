
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

TEST(QueenTest, MovesEmpty) {
    Pieces::Queen queen;
    auto moves = queen.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(QueenTest, MovesCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 4;
    int column = 2;
    Pieces::Queen queen(Position(row, column));
    Position initial = queen.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&queen, initial, Position(row + 1, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 2, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 3, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 1, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 2, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 3, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 4, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&queen, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 4), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 5), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 2), moveType),
            // Bottom Left Diagonal
            Pieces::Move::createMove(&queen, initial, Position(row - 1, column - 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 2, column - 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 1, column + 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 2, column + 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 3, column + 3), moveType),
            // Bottom Right Diagonal
            Pieces::Move::createMove(&queen, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 2, column - 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 1, column + 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 2, column + 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 3, column + 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 4, column + 4), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = queen.moves(nRow, nColumn);
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

TEST(QueenTest, MoveTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Queen queen(Position(row, column));
    Position initial = queen.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&queen, initial, Position(row - 1, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 2, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 3, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 4, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 5, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 6, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 7, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&queen, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 4), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 5), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 6), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column + 7), moveType),
            // Bottom Left Diagonal: None
            // Bottom Right Diagonal
            Pieces::Move::createMove(&queen, initial, Position(row - 1, column + 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 2, column + 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 3, column + 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 4, column + 4), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 5, column + 5), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 6, column + 6), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row - 7, column + 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = queen.moves(nRow, nColumn);
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

TEST(QueenTest, MoveBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Queen queen(Position(row, column));
    Position initial = queen.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Vertical moves
            Pieces::Move::createMove(&queen, initial, Position(row + 1, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 2, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 3, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 4, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 5, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 6, column), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 7, column), moveType),
            // Horizontal moves
            Pieces::Move::createMove(&queen, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 4), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 5), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 6), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row, column - 7), moveType),
            // Bottom Left Diagonal: None
            // Bottom Right Diagonal
            Pieces::Move::createMove(&queen, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 2, column - 2), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 3, column - 3), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 4, column - 4), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 5, column - 5), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 6, column - 6), moveType),
            Pieces::Move::createMove(&queen, initial, Position(row + 7, column - 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = queen.moves(nRow, nColumn);
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
