#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"

TEST(KingTest, DefaultConstructor) {
    Pieces::King king;
    EXPECT_EQ(typeid(king.position()), typeid(Position));
    EXPECT_EQ(king.nMoves(), 0);
}

TEST(KingTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::King king(pos);
    EXPECT_EQ(king.position(), pos);
    EXPECT_EQ(king.nMoves(), 0);
}

TEST(KingTest, MovesEmpty) {
    Pieces::King king;
    auto moves = king.moves(0, 0);
    EXPECT_TRUE(moves.empty());
}

TEST(KingTest, Moves_SurroundedByOpponents) {
    int row = 3;
    int column = 3;
    Pieces::King king(Position(row, column));

    std::vector<Pieces::Rook> opponents = {
        Pieces::Rook(Position(row + 1, column - 1)), Pieces::Rook(Position(row + 1, column + 1)),
        Pieces::Rook(Position(row - 1, column - 1)), Pieces::Rook(Position(row - 1, column + 1))};
    std::vector<Pieces::Piece *> pOpponents;
    for (auto &opponent : opponents) {
        pOpponents.push_back(&opponent);
    }

    auto moves = king.moves(8, 8, pOpponents);
    EXPECT_EQ(moves.size(), 0);
}

TEST(KingTest, Moves_WhenPieceAtTheCenter) {
    int nRow = 8;
    int nColumn = 8;
    int row = 3;
    int column = 3;
    Pieces::King king(Position(row, column));
    Position initial = king.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&king, initial, Position(row + 1, column), moveType),
            Pieces::Move::createMove(&king, initial, Position(row - 1, column), moveType),
            Pieces::Move::createMove(&king, initial, Position(row + 1, column + 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row - 1, column - 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row - 1, column + 1), moveType),
        };
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = king.moves(nRow, nColumn);
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

TEST(KingTest, Moves_WhenPieceAtTopLeft) {
    int nRow = 8;
    int nColumn = 8;
    int row = nRow - 1;
    int column = 0;
    Pieces::King king(Position(row, column));
    Position initial = king.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&king, initial, Position(row, column + 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row - 1, column + 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row - 1, column), moveType),
        };
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = king.moves(nRow, nColumn);
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

TEST(KingTest, Moves_WhenPieceAtBottomRight) {
    int nRow = 8;
    int nColumn = 8;
    int row = 0;
    int column = nColumn - 1;
    Pieces::King king(Position(row, column));
    Position initial = king.position();
    std::vector<Pieces::Move> expectedMoves;
    std::vector<Pieces::Move::Type> moveTypes = {Pieces::Move::Type::DISPLACEMENT,
                                                 Pieces::Move::Type::CAPTURE};
    for (const auto &moveType : moveTypes) {
        std::vector<Pieces::Move> newMoves = {
            Pieces::Move::createMove(&king, initial, Position(row, column - 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row + 1, column - 1), moveType),
            Pieces::Move::createMove(&king, initial, Position(row + 1, column), moveType),
        };
        expectedMoves.insert(expectedMoves.end(), newMoves.begin(), newMoves.end());
    }

    auto moves = king.moves(nRow, nColumn);
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
