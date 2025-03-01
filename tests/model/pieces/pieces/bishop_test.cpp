#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;

class BishopTest : public PiecesTest {
  protected:
    Pieces::Bishop bishop;

    void SetUp() override {
        PiecesTest::SetUp();
        bishop = Pieces::Bishop(initialPosition);
    }
};

TEST_F(BishopTest, DefaultConstructor) {
    Pieces::Bishop bishop;
    EXPECT_EQ(bishop.type(), Pieces::Types::BISHOP);
    EXPECT_EQ(typeid(bishop.position()), typeid(Position));
    EXPECT_EQ(bishop.nMoves(), 0);
}

TEST_F(BishopTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Bishop bishop(pos);
    EXPECT_EQ(bishop.type(), Pieces::Types::BISHOP);
    EXPECT_EQ(bishop.position(), pos);
    EXPECT_EQ(bishop.nMoves(), 0);
}

TEST_F(BishopTest, MovesEmpty) {
    moves = bishop.moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(BishopTest, Moves_AvailableDisplacements) {
    std::vector<Position> expectedPositions = {// DownLeft to TopRight
                                               Position(0, 0), Position(1, 1), Position(2, 2),
                                               Position(4, 4), Position(5, 5), Position(6, 6),
                                               Position(7, 7),
                                               // DownRight to TopLeft
                                               Position(6, 0), Position(5, 1), Position(4, 2),
                                               Position(2, 4), Position(1, 5), Position(0, 6)};

    moves = bishop.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));
    }
}

TEST_F(BishopTest, Moves_AvailableCaptures) {
    std::vector<Position> displacementPositions = {
        Position(4, 2),
        Position(4, 4),
        Position(5, 5),
    };

    std::vector<Position> opponentPositions = {Position(5, 1), Position(6, 6), Position(2, 4),
                                               Position(2, 2)};
    for (const auto &position : opponentPositions) {
        addOpponentAt(position);
    }

    moves = bishop.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size() + opponentPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));
    }
    for (const auto &position : opponentPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::CAPTURE);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(expectedPiece2, *opponents[position]);
        ASSERT_EQ(typeid(expectedPiece2), typeid(MockPiece1));
    }
}

TEST_F(BishopTest, Moves_StopAtFriendlies) {
    std::vector<Position> displacementPositions = {
        Position(4, 2),
        Position(4, 4),
        Position(5, 5),
    };

    std::vector<Position> friendlyPositions = {Position(5, 1), Position(6, 6), Position(2, 4),
                                               Position(2, 2)};
    for (const auto &position : friendlyPositions) {
        addFriendlyAt(position);
    }

    moves = bishop.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));
    }
}

TEST_F(BishopTest, Moves_TopLeft) {
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);
    bishop = Pieces::Bishop(corner);

    std::vector<Position> displacementPositions = {// DownRight to TopLeft
                                                   Position(6, 1), Position(5, 2), Position(4, 3),
                                                   Position(3, 4), Position(2, 5), Position(1, 6),
                                                   Position(0, 7)};

    moves = bishop.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(bishop.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));
    }
}

TEST_F(BishopTest, Moves_BottomRight) {
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);
    bishop = Pieces::Bishop(corner);

    std::vector<Position> displacementPositions = {// DownRight to TopLeft
                                                   Position(7, 0), Position(6, 1), Position(5, 2),
                                                   Position(4, 3), Position(3, 4), Position(2, 5),
                                                   Position(1, 6)};

    moves = bishop.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(bishop.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, bishop);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Bishop));
    }
}
