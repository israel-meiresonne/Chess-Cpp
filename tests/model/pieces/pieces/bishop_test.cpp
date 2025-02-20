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

TEST(BishopTest, MovesEmpty) {
    Pieces::Bishop bishop;
    auto moves = bishop.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(BishopTest, MovesCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 4;
    int column = 2;
    Pieces::Bishop bishop(Position(row, column));
    Position initial = bishop.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Bottom Left Diagonal
            Pieces::Move::createMove(&bishop, initial, Position(row - 1, column - 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 2, column - 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 1, column + 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 2, column + 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 3, column + 3), moveType),
            // Bottom Right Diagonal
            Pieces::Move::createMove(&bishop, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 2, column - 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 1, column + 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 2, column + 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 3, column + 3), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 4, column + 4), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = bishop.moves(nRow, nColumn);
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

TEST(BishopTest, MoveTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Bishop bishop(Position(row, column));
    Position initial = bishop.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Bottom Left Diagonal: None
            // Bottom Right Diagonal
            Pieces::Move::createMove(&bishop, initial, Position(row - 1, column + 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 2, column + 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 3, column + 3), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 4, column + 4), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 5, column + 5), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 6, column + 6), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row - 7, column + 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = bishop.moves(nRow, nColumn);
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

TEST(BishopTest, MoveBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Bishop bishop(Position(row, column));
    Position initial = bishop.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            // Bottom Left Diagonal: None
            // Bottom Right Diagonal
            Pieces::Move::createMove(&bishop, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 2, column - 2), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 3, column - 3), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 4, column - 4), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 5, column - 5), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 6, column - 6), moveType),
            Pieces::Move::createMove(&bishop, initial, Position(row + 7, column - 7), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = bishop.moves(nRow, nColumn);
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
