#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;

class KingTest : public PiecesTest {
  protected:
    Pieces::King *king;

    void SetUp() override {
        PiecesTest::SetUp();
        king = new Pieces::King(initialPosition);
        friendlies[king->position()] = king;
    }

    void addFriendlyRookAt(::Position position) {
        friendlies[position] = new Pieces::Rook(position);
    }

    void addFriendlyBishopAt(::Position position) {
        friendlies[position] = new Pieces::Bishop(position);
    }

    void addOpponentBishopAt(::Position position) {
        opponents[position] = new Pieces::Bishop(position);
    }

    void addOpponentPawnAt(::Position position) {
        opponents[position] = new Pieces::Pawn(position);
    }
};

TEST_F(KingTest, DefaultConstructor) {
    Pieces::King king;
    EXPECT_EQ(king.type(), Pieces::Types::KING);
    EXPECT_EQ(typeid(king.position()), typeid(Position));
    EXPECT_EQ(king.nMoves(), 0);
}

TEST_F(KingTest, ParameterizedConstructor) {
    Position pos(2, 3);
    Pieces::King king(pos);
    EXPECT_EQ(king.type(), Pieces::Types::KING);
    EXPECT_EQ(king.position(), pos);
    EXPECT_EQ(king.nMoves(), 0);
}

TEST_F(KingTest, MovesEmpty) {
    moves = king->moves(friendlies, 0, 0, opponents);
    EXPECT_TRUE(moves.empty());
}

TEST_F(KingTest, Moves_AvailableDisplacements) {
    std::vector<Position> displacement = {Position(4, 2), Position(4, 3), Position(4, 4),
                                          Position(3, 4), Position(3, 2), Position(2, 2),
                                          Position(2, 3), Position(2, 4)};

    moves = king->moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacement.size());
    for (const auto &position : displacement) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, *king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_AvailableCaptures) {
    std::vector<Position> displacement = {Position(4, 4), Position(2, 2), Position(2, 4)};

    std::vector<Position> opponentPositions = {Position(4, 2), Position(3, 4), Position(2, 3)};
    std::vector<Position> opponentCapturable = {Position(4, 2)};

    for (const auto &position : opponentPositions) {
        addOpponentBishopAt(position);
    }

    moves = king->moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacement.size() + opponentCapturable.size());
    for (const auto &position : displacement) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, *king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
    for (const auto &position : opponentCapturable) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::CAPTURE);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, *king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(expectedPiece2, *opponents[position]);
        ASSERT_EQ(typeid(expectedPiece2), typeid(Pieces::Bishop));
    }
}

TEST_F(KingTest, Moves_StopAtFriendlies) {
    std::vector<Position> displacement = {Position(4, 3), Position(4, 4), Position(3, 2),
                                          Position(2, 2), Position(2, 4)};
    std::vector<Position> friendlyPositions = {Position(4, 2), Position(3, 4), Position(2, 3)};

    for (const auto &position : friendlyPositions) {
        addFriendlyAt(position);
    }

    moves = king->moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), displacement.size());
    for (const auto &position : displacement) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, *king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_TopLeft) {
    Position corner(nRow - 1, 0);
    Position outOfBounds1(corner.row() + 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() - 1);
    Pieces::King king = Pieces::King(corner);

    std::vector<Position> displacement = {Position(7, 1), Position(6, 1), Position(6, 0)};

    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(king.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacement.size());
    for (const auto &position : displacement) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_BottomRight) {
    Position corner(0, nColumn - 1);
    Position outOfBounds1(corner.row() - 1, corner.column());
    Position outOfBounds2(corner.row(), corner.column() + 1);
    Pieces::King king = Pieces::King(corner);

    std::vector<Position> displacement = {Position(0, 6), Position(1, 6), Position(1, 7)};

    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_TRUE(Pieces::Piece::isInBounds(king.position(), nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds1, nRow, nColumn));
    EXPECT_FALSE(Pieces::Piece::isInBounds(outOfBounds2, nRow, nColumn));

    EXPECT_EQ(moves.size(), displacement.size());
    for (const auto &position : displacement) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_Castling) {
    Pieces::King king = Pieces::King(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5),
                                               Position(0, 5), Position(0, 3)};

    std::vector<Position> castingPositions = {Position(0, 2), Position(0, 6)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
    }
    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size() + castingPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
    for (const auto &position : castingPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::SWAP);
        EXPECT_EQ(moves.at(position).actions().size(), 2);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));

        auto &expectedPiece2 = *moves.at(position).actions()[1].piece();
        ASSERT_EQ(typeid(expectedPiece2), typeid(Pieces::Rook));
    }
}

TEST_F(KingTest, Moves_CastlingWhenKingHasMoved) {
    king->move(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5),
                                               Position(0, 5), Position(0, 3)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
    }
    moves = king->moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, *king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_CastlingWhenRookHasMoved) {
    Pieces::King king = Pieces::King(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5),
                                               Position(0, 5), Position(0, 3)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
        friendlies.at(position)->move(position);
    }
    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_CastlingWhenPathToRookIsNotFree) {
    // Deactivate until Piece become abstract again
    Pieces::King king = Pieces::King(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5),
                                               Position(0, 5), Position(0, 3)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
    }
    addFriendlyBishopAt(Position(0, 6));
    addOpponentBishopAt(Position(0, 1));

    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_CastlingWhenKingFinalPositionIsThreatened) {
    Pieces::King king = Pieces::King(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5),
                                               Position(0, 5), Position(0, 3)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
    }
    addOpponentBishopAt(Position(2, 0));
    addOpponentBishopAt(Position(1, 7));

    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}

TEST_F(KingTest, Moves_CastlingWhenRookFinalPositionIsThreatened) {
    Pieces::King king = Pieces::King(Position(0, 4));
    std::vector<Position> expectedPositions = {Position(1, 3), Position(1, 4), Position(1, 5)};

    std::vector<Position> rookPositions = {Position(0, 0), Position(0, 7)};

    for (const auto &position : rookPositions) {
        addFriendlyRookAt(position);
    }
    addOpponentBishopAt(Position(3, 0));
    addOpponentBishopAt(Position(2, 7));

    moves = king.moves(friendlies, nRow, nColumn, opponents);

    EXPECT_EQ(moves.size(), expectedPositions.size());
    for (const auto &position : expectedPositions) {
        EXPECT_TRUE(moves.count(position));
        EXPECT_TRUE(moves.at(position).type() == Pieces::Move::Type::DISPLACEMENT);
        EXPECT_EQ(moves.at(position).actions().size(), 1);

        auto &expectedPiece1 = *moves.at(position).actions()[0].piece();
        ASSERT_EQ(expectedPiece1, king);
        ASSERT_EQ(typeid(expectedPiece1), typeid(Pieces::King));
    }
}
