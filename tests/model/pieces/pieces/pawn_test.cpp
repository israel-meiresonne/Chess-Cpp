
#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;

class PawnTest : public PiecesTest {
  protected:
    Pieces::Pawn pawn;

    void SetUp() override {
        PiecesTest::SetUp();
        pawn = Pieces::Pawn(initialPosition);
    }

    std::vector<Position> addOpponentsFrom(Position position) {
        std::vector<Position> opponents;
        Position opponentPosition1(position.row() + 1, position.column() - 1);
        Position opponentPosition2(position.row() + 1, position.column() + 1);

        opponents.push_back(opponentPosition1);
        opponents.push_back(opponentPosition2);

        for (auto &opponent : opponents) {
            addOpponentAt(opponent);
        }
        return opponents;
    }
};

TEST_F(PawnTest, DefaultConstructor) {
    Pieces::Pawn pawn;
    EXPECT_EQ(pawn.type(), Pieces::Types::PAWN);
    EXPECT_EQ(typeid(pawn.position()), typeid(Position));
    EXPECT_EQ(pawn.nMoves(), 0);
    EXPECT_THROW(pawn.player(), std::runtime_error);
}

TEST_F(PawnTest, ParameterizedConstructor) {
    Position position(2, 3);
    Pieces::Player player("Alice");
    Pieces::Pawn pawn(position, &player);

    EXPECT_EQ(pawn.type(), Pieces::Types::PAWN);
    EXPECT_EQ(pawn.position(), position);
    EXPECT_EQ(pawn.nMoves(), 0);
    EXPECT_EQ(pawn.player(), &player);
}

TEST_F(PawnTest, MovesEmpty) {
    moves = pawn.moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(PawnTest, Moves_InitialMoveAllowsOneOrTwoSteps) {
    int nRow = 8, nColumn = 8;
    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    Position oneStep(4, 3);
    Position twoSteps(5, 3);

    EXPECT_EQ(moves.size(), 2);
    EXPECT_TRUE(moves.count(oneStep));
    EXPECT_TRUE(moves.count(twoSteps));
}

TEST_F(PawnTest, Moves_MoveRestrictedAfterFirstMove) {
    int nRow = 8, nColumn = 8;
    Position firstFinalPosition(4, 3);
    Position secondFinalPosition(5, 3);

    pawn.move(firstFinalPosition);
    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), 1);
    EXPECT_TRUE(moves.count(secondFinalPosition));
}

TEST_F(PawnTest, Moves_CaptureMoveAvailable) {
    int nRow = 8, nColumn = 8;
    Position oneStep(4, 3);
    Position twoSteps(5, 3);
    auto opponentPositions = addOpponentsFrom(pawn.position());

    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), 4);
    EXPECT_TRUE(moves.count(oneStep));
    EXPECT_TRUE(moves.count(twoSteps));
    for (auto &opponentPosition : opponentPositions) {
        EXPECT_TRUE(moves.count(opponentPosition));
    }
}

TEST_F(PawnTest, Moves_CannotMoveThroughPiece) {
    int nRow = 8, nColumn = 8;
    Position twoSteps(5, 3);
    Position blockingPosition(4, 3);
    Pieces::Pawn *blocker = new Pieces::Pawn(blockingPosition);
    friendlies[blockingPosition] = blocker;

    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), 0);
    EXPECT_FALSE(moves.count(twoSteps));
}

TEST_F(PawnTest, Moves_TopLeft) {
    int nRow = 8, nColumn = 8;
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);

    Pieces::Pawn pawn(corner);
    addOpponentsFrom(pawn.position());

    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, Moves_BottomRight) {
    int nRow = 8, nColumn = 8;
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);

    Pieces::Pawn pawn(corner);
    addOpponentsFrom(pawn.position());

    Position opponentPosition1(1, 6);
    Position opponentPosition2(1, nRow);
    Position oneStep(1, 7);
    Position twoSteps(2, 7);

    moves = pawn.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(pawn.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), 3);
    EXPECT_TRUE(moves.count(oneStep));
    EXPECT_TRUE(moves.count(twoSteps));
    EXPECT_TRUE(moves.count(opponentPosition1));
    EXPECT_FALSE(moves.count(opponentPosition2));
}
