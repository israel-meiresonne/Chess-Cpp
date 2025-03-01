
#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;

class KnightTest : public PiecesTest {
  protected:
    Pieces::Knight knight;

    void SetUp() override {
        PiecesTest::SetUp();
        knight = Pieces::Knight(initialPosition);
    }
};

TEST_F(KnightTest, DefaultConstructor) {
    Pieces::Knight knight;
    EXPECT_EQ(knight.type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(typeid(knight.position()), typeid(Position));
    EXPECT_EQ(knight.nMoves(), 0);
}

TEST_F(KnightTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::Knight knight(pos);
    EXPECT_EQ(knight.type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(knight.position(), pos);
    EXPECT_EQ(knight.nMoves(), 0);
}

TEST_F(KnightTest, MovesEmpty) {
    moves = knight.moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(KnightTest, Moves_AvailableDisplacements) {
    std::vector<Position> expectedPositions = {Position(5, 2), Position(5, 4), Position(4, 5),
                                               Position(2, 5), Position(1, 4), Position(1, 2),
                                               Position(2, 1), Position(4, 1)};

    moves = knight.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));
    }
}

TEST_F(KnightTest, Moves_AvailableCaptures) {
    std::vector<Position> displacementPositions = {Position(5, 4), Position(4, 5), Position(1, 2),
                                                   Position(4, 1)};

    std::vector<Position> opponentPositions = {Position(5, 2), Position(2, 5), Position(1, 4),
                                               Position(2, 1)};

    for (const auto &position : opponentPositions) {
        addOpponentAt(position);
    }

    moves = knight.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size() + opponentPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));
    }
    for (const auto &position : opponentPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::CAPTURE);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(expectedPiece2, *opponents[position]);
        ASSERT_EQ(typeid(expectedPiece2), typeid(MockPiece1));
    }
}

TEST_F(KnightTest, Moves_StopAtFriendlies) {
    std::vector<Position> displacementPositions = {Position(5, 4), Position(4, 5), Position(1, 2),
                                                   Position(4, 1)};
    std::vector<Position> friendlyPositions = {Position(5, 2), Position(2, 5), Position(1, 4),
                                               Position(2, 1)};

    for (const auto &position : friendlyPositions) {
        addFriendlyAt(position);
    }

    moves = knight.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));
    }
}

TEST_F(KnightTest, Moves_TopLeft) {
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);
    knight = Pieces::Knight(corner);

    std::vector<Position> displacementPositions = {Position(6, 2), Position(5, 1)};

    moves = knight.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(knight.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));
    }
}

TEST_F(KnightTest, Moves_BottomRight) {
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);
    knight = Pieces::Knight(corner);

    std::vector<Position> displacementPositions = {Position(2, 6), Position(1, 5)};

    moves = knight.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(knight.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, knight);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Knight));
    }
}
