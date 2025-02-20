
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

TEST(KnightTest, MovesEmpty) {
    Pieces::Knight knight;
    auto moves = knight.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(KnightTest, MovesCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 3;
    int column = 3;
    Pieces::Knight knight(Position(row, column));
    Position initial = knight.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&knight, initial, Position(row + 2, column + 1), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row + 2, column - 1), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row - 2, column + 1), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row - 2, column - 1), moveType),

            Pieces::Move::createMove(&knight, initial, Position(row + 1, column + 2), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row - 1, column + 2), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row + 1, column - 2), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row - 1, column - 2), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = knight.moves(nRow, nColumn);
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

TEST(KnightTest, MoveTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::Knight knight(Position(row, column));
    Position initial = knight.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&knight, initial, Position(row - 2, column + 1), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row - 1, column + 2), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = knight.moves(nRow, nColumn);
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

TEST(KnightTest, MoveBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::Knight knight(Position(row, column));
    Position initial = knight.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&knight, initial, Position(row + 2, column - 1), moveType),
            Pieces::Move::createMove(&knight, initial, Position(row + 1, column - 2), moveType)};
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = knight.moves(nRow, nColumn);
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
