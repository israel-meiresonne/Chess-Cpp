#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;

class QueenTest : public PiecesTest {
  protected:
    Pieces::Queen queen;

    void SetUp() override {
        PiecesTest::SetUp();
        queen = Pieces::Queen(initialPosition);
    }
};

TEST_F(QueenTest, DefaultConstructor) {
    Pieces::Queen queen;
    EXPECT_EQ(queen.type(), Pieces::Types::QUEEN);
    EXPECT_EQ(typeid(queen.position()), typeid(Position));
    EXPECT_EQ(queen.nMoves(), 0);
    EXPECT_THROW(queen.player(), std::runtime_error);
}

TEST_F(QueenTest, ParameterizedConstructor) {
    Position position(2, 3);
    Pieces::Player player("Alice");
    Pieces::Queen queen(position, &player);

    EXPECT_EQ(queen.type(), Pieces::Types::QUEEN);
    EXPECT_EQ(queen.position(), position);
    EXPECT_EQ(queen.nMoves(), 0);
    EXPECT_EQ(queen.player(), &player);
}

TEST_F(QueenTest, MovesEmpty) {
    moves = queen.moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(QueenTest, Moves_AvailableDisplacements) {
    std::vector<Position> expectedPositions = {
        // Rook moves
        Position(4, 3), Position(5, 3), Position(6, 3), Position(7, 3), Position(2, 3),
        Position(1, 3), Position(0, 3), Position(3, 4), Position(3, 5), Position(3, 6),
        Position(3, 7), Position(3, 2), Position(3, 1), Position(3, 0),
        // Bishop moves
        Position(4, 4), Position(5, 5), Position(6, 6), Position(7, 7), Position(2, 2),
        Position(1, 1), Position(0, 0), Position(2, 4), Position(1, 5), Position(0, 6),
        Position(4, 2), Position(5, 1), Position(6, 0)};

    moves = queen.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));
    }
}

TEST_F(QueenTest, Moves_AvailableCaptures) {
    std::vector<Position> displacementPositions = {
        Position(6, 0), Position(3, 0), Position(5, 1), Position(3, 1), Position(4, 2),
        Position(3, 2), Position(4, 3), Position(2, 4), Position(3, 4), Position(4, 4),
        Position(2, 3), Position(3, 5), Position(5, 5), Position(1, 3), Position(0, 3)};
    std::vector<Position> opponentPositions = {Position(5, 3), Position(2, 2), Position(1, 5),
                                               Position(3, 6), Position(6, 6)};

    for (const auto &position : opponentPositions) {
        addOpponentAt(position);
    }

    moves = queen.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size() + opponentPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));
    }
    for (const auto &position : opponentPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::CAPTURE);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(expectedPiece2, *opponents[position]);
        ASSERT_EQ(typeid(expectedPiece2), typeid(MockPiece1));
    }
}

TEST_F(QueenTest, Moves_StopAtFriendlies) {
    std::vector<Position> displacementPositions = {
        Position(6, 0), Position(3, 0), Position(5, 1), Position(3, 1), Position(4, 2),
        Position(3, 2), Position(4, 3), Position(2, 4), Position(3, 4), Position(4, 4),
        Position(2, 3), Position(3, 5), Position(5, 5), Position(1, 3), Position(0, 3)};
    std::vector<Position> friendlyPositions = {Position(5, 3), Position(2, 2), Position(1, 5),
                                               Position(3, 6), Position(6, 6)};

    for (const auto &position : friendlyPositions) {
        addFriendlyAt(position);
    }

    moves = queen.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));
    }
}

TEST_F(QueenTest, Moves_TopLeft) {
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);
    queen = Pieces::Queen(corner);

    std::vector<Position> displacementPositions = {
        // Horizontal and Vertical (Rook-like moves)
        Position(7, 1), Position(7, 2), Position(7, 3), Position(7, 4), Position(7, 5),
        Position(7, 6), Position(7, 7), Position(6, 0), Position(5, 0), Position(4, 0),
        Position(3, 0), Position(2, 0), Position(1, 0), Position(0, 0),
        // Diagonal (Bishop-like moves)
        Position(6, 1), Position(5, 2), Position(4, 3), Position(3, 4), Position(2, 5),
        Position(1, 6), Position(0, 7)};

    moves = queen.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(queen.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));
    }
}

TEST_F(QueenTest, Moves_BottomRight) {
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);
    queen = Pieces::Queen(corner);

    std::vector<Position> displacementPositions = {
        // Horizontal and Vertical (Rook-like moves)
        Position(0, 0), Position(0, 1), Position(0, 2), Position(0, 3), Position(0, 4),
        Position(0, 5), Position(0, 6), Position(1, 7), Position(2, 7), Position(3, 7),
        Position(4, 7), Position(5, 7), Position(6, 7), Position(7, 7),
        // Diagonal (Bishop-like moves)
        Position(7, 0), Position(6, 1), Position(5, 2), Position(4, 3), Position(3, 4),
        Position(2, 5), Position(1, 6)};

    moves = queen.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(queen.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacementPositions.size());
    for (const auto &position : displacementPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, queen);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::Queen));
    }
}
