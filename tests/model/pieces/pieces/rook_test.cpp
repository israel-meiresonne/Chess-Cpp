
#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;

class RookTest : public PiecesTest {
  protected:
    Pieces::Rook rook;

    void SetUp() override {
        PiecesTest::SetUp();
        rook = Pieces::Rook(initialPosition);
    }
};

TEST_F(RookTest, DefaultConstructor) {
    Pieces::Rook rook;
    EXPECT_EQ(rook.type(), Pieces::Types::ROOK);
    EXPECT_EQ(typeid(rook.position()), typeid(Position));
    EXPECT_EQ(rook.nMoves(), 0);
    EXPECT_THROW(rook.player(), std::runtime_error);
}

TEST_F(RookTest, ParameterizedConstructor) {
    Position position(2, 3);
    Pieces::Player player("Alice");
    Pieces::Rook rook(position, &player);

    EXPECT_EQ(rook.type(), Pieces::Types::ROOK);
    EXPECT_EQ(rook.position(), position);
    EXPECT_EQ(rook.nMoves(), 0);
    EXPECT_EQ(rook.player(), &player);
}

TEST_F(RookTest, MovesEmpty) {
    moves = rook.moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(RookTest, Moves_AvailableDisplacements) {
    std::vector<Position> expectedPositions = {
        Position(4, 3), Position(5, 3), Position(6, 3), Position(7, 3), Position(2, 3),
        Position(1, 3), Position(0, 3), Position(3, 4), Position(3, 5), Position(3, 6),
        Position(3, 7), Position(3, 2), Position(3, 1), Position(3, 0)};

    moves = rook.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece = *moves.at(position).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == rook);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(rook));
    }
}

TEST_F(RookTest, Moves_AvailableCaptures) {
    std::vector<Position> displacementPositions = {Position(4, 3), Position(3, 4), Position(3, 5)};

    std::vector<Position> opponentPositions = {Position(5, 3), Position(3, 6), Position(2, 3),
                                               Position(3, 2)};
    for (const auto &position : opponentPositions) {
        addOpponentAt(position);
    }

    moves = rook.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size() + opponentPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece = *moves.at(position).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == rook);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(rook));
    }
    for (const auto &position : opponentPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::CAPTURE);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, rook);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Rook));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(expectedPiece2, *opponents[position]);
        ASSERT_EQ(typeid(expectedPiece2), typeid(MockPiece1));
    }
}

TEST_F(RookTest, Moves_StopAtFriendlies) {
    std::vector<Position> displacementPositions = {Position(4, 3), Position(3, 4), Position(3, 5)};

    std::vector<Position> friendlyPositions = {Position(5, 3), Position(3, 6), Position(2, 3),
                                               Position(3, 2)};
    for (const auto &position : friendlyPositions) {
        addFriendlyAt(position);
    }

    moves = rook.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_TRUE(expectedPiece1 == rook);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Rook));
    }
}

TEST_F(RookTest, Moves_TopLeft) {
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);
    rook = Pieces::Rook(corner);

    std::vector<Position> displacementPositions = {
        // Right
        Position(7, 1), Position(7, 2), Position(7, 3), Position(7, 4), Position(7, 5),
        Position(7, 6), Position(7, 7),
        // Down
        Position(0, 0), Position(1, 0), Position(2, 0), Position(3, 0), Position(4, 0),
        Position(5, 0), Position(6, 0)};

    moves = rook.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(rook.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_TRUE(expectedPiece1 == rook);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Rook));
    }
}

TEST_F(RookTest, Moves_BottomRight) {
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);
    rook = Pieces::Rook(corner);

    std::vector<Position> displacementPositions = {
        // Up
        Position(1, 7), Position(2, 7), Position(3, 7), Position(4, 7), Position(5, 7),
        Position(6, 7), Position(7, 7),
        // Left
        Position(0, 0), Position(0, 1), Position(0, 2), Position(0, 3), Position(0, 4),
        Position(0, 5), Position(0, 6)};

    moves = rook.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(rook.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_TRUE(expectedPiece1 == rook);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Rook));
    }
}
