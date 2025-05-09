#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/pieces/pieces_test.hpp>

using PiecesTest = ::Tests::Pieces::PiecesTest;
using MockPiece1 = ::Tests::Pieces::MockPiece1;
using MockPiece2 = ::Tests::Pieces::MockPiece2;
using MockPiece3 = ::Tests::Pieces::MockPiece3;

class PieceTest : public PiecesTest {
  protected:
    Position initialPosition;
    MockPiece1 piece;
    std::unordered_map<Position, ::Pieces::Move> moves;
    std::unordered_map<Position, ::Pieces::Move> captures;

    void SetUp() override {
        initialPosition = Position(3, 3);
        piece = MockPiece1(initialPosition);
        piece.callSetFriendlies(friendlies);
        piece.callSetOpponents(opponents);
    }
};

TEST_F(PieceTest, DefaultConstructor) {
    MockPiece1 piece;
    EXPECT_EQ(piece.type(), Pieces::Types::UNDEFINED);
    EXPECT_EQ(typeid(piece.position()), typeid(Position));
    EXPECT_EQ(piece.nMoves(), 0);
    EXPECT_THROW(piece.owner(), std::runtime_error);
}

TEST_F(PieceTest, PositionConstructor) {
    Position pos(2, 3);
    MockPiece1 piece(pos);
    EXPECT_EQ(piece.type(), Pieces::Types::UNDEFINED);
    EXPECT_EQ(piece.position(), pos);
    EXPECT_EQ(piece.nMoves(), 0);
    EXPECT_THROW(piece.owner(), std::runtime_error);
}

TEST_F(PieceTest, Constructor_WithPositionAndPlayer) {
    Position position(2, 3);
    Pieces::Player player("Alice");
    MockPiece1 piece(position, &player);

    EXPECT_EQ(piece.position(), position);
    EXPECT_EQ(piece.type(), Pieces::Types::UNDEFINED);
    EXPECT_EQ(piece.owner(), &player);
}

TEST_F(PieceTest, Getter_Opponents) {
    Position position(2, 3);
    Position position2(3, 2);
    MockPiece3 *piece = new MockPiece3(position);
    MockPiece3 *piece2 = new MockPiece3(position);
    friendlies[position2] = piece2;
    opponents[position] = piece;

    std::unordered_map<Position, Pieces::Move> moves = piece->moves(friendlies, 8, 8, opponents);

    EXPECT_THROW(piece->callOpponents(), std::runtime_error);
    EXPECT_EQ(moves.size(), 0);
}

TEST_F(PieceTest, Getter_Friendlies) {
    Position position(2, 3);
    Position position2(3, 2);
    MockPiece3 *piece = new MockPiece3(position);
    MockPiece3 *piece2 = new MockPiece3(position2);
    friendlies[position2] = piece2;
    opponents[position] = piece;

    std::unordered_map<Position, Pieces::Move> moves = piece->moves(friendlies, 8, 8, opponents);

    EXPECT_THROW(piece->callFriendlies(), std::runtime_error);
    EXPECT_EQ(moves.size(), 0);
}

TEST_F(PieceTest, Move_IncrementsMoveCount) {
    Position pos1(1, 1);
    Position pos2(2, 2);
    Position pos3(3, 3);
    MockPiece1 piece(pos1);

    piece.move(pos2);
    EXPECT_EQ(piece.position(), pos2);
    EXPECT_EQ(piece.nMoves(), 1);

    piece.move(pos3);
    EXPECT_EQ(piece.position(), pos3);
    EXPECT_EQ(piece.nMoves(), 2);
}

TEST_F(PieceTest, genMovesInDirection_GeneratesMovesInGivenDirection) {
    Position endPosition(6, 6);
    Pieces::Move::Direction direction = Pieces::Move::Direction::UP_RIGHT;

    piece._genMovesInDirection(moves, endPosition, direction, captures);

    EXPECT_FALSE(moves.empty());
    EXPECT_EQ(moves.size(), 3);
    EXPECT_TRUE(moves.count(endPosition));
    EXPECT_TRUE(moves.count(Position(5, 5)));
    EXPECT_TRUE(moves.count(Position(4, 4)));
    EXPECT_FALSE(moves.count(piece.position()));
}

TEST_F(PieceTest, genMovesInDirection_StopsAtFriendlyPiece) {
    Position endPosition(6, 6);
    Pieces::Move::Direction direction = Pieces::Move::Direction::UP_RIGHT;
    Position friendlyPosition(5, 5);
    MockPiece1 *friendly = new MockPiece1(friendlyPosition);
    friendlies[friendlyPosition] = friendly;

    piece._genMovesInDirection(moves, endPosition, direction, captures);

    EXPECT_EQ(moves.count(friendlyPosition), 0);
    EXPECT_EQ(moves.count(endPosition), 0);
}

TEST_F(PieceTest, genMovesInDirection_CapturesOpponentPiece) {
    Position endPosition(6, 6);
    Pieces::Move::Direction direction = Pieces::Move::Direction::UP_RIGHT;
    Position opponentPosition(5, 5);
    MockPiece1 *opponentPiece = new MockPiece1(opponentPosition);
    opponents[opponentPosition] = opponentPiece;
    captures[opponentPosition] = Pieces::Move(Pieces::Move::Type::CAPTURE);

    piece._genMovesInDirection(moves, endPosition, direction, captures);

    EXPECT_EQ(moves.count(opponentPosition), 1);
    EXPECT_EQ(moves[opponentPosition].type(), Pieces::Move::Type::CAPTURE);
}

TEST_F(PieceTest, genMovesInDirection_DoesNotMoveOutOfBounds) {
    Position endPosition(8, 8);
    Pieces::Move::Direction direction = Pieces::Move::Direction::UP_RIGHT;

    piece._genMovesInDirection(moves, endPosition, direction, captures);

    for (const auto &[pos, move] : moves) {
        EXPECT_TRUE(pos.row() >= 0 && pos.row() <= 8);
        EXPECT_TRUE(pos.column() >= 0 && pos.column() <= 8);
    }
}

TEST_F(PieceTest, genCapturesInDirection_PieceIsAtEndPosition) {
    Position end = piece.position();

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::UP);

    ASSERT_TRUE(moves.empty());
}

TEST_F(PieceTest, genCapturesInDirection_UP_LEFT) {
    Position end(5, 1);
    std::vector<Position> expectedPositions = {Position(4, 2), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::UP_LEFT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}
TEST_F(PieceTest, genCapturesInDirection_UP) {
    Position end(6, 3);
    std::vector<Position> expectedPositions = {Position(4, 3), Position(5, 3), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::UP);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_UP_RIGHT) {
    Position end(6, 6);
    std::vector<Position> expectedPositions = {Position(4, 4), Position(5, 5), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::UP_RIGHT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_RIGHT) {
    Position end(3, 6);
    std::vector<Position> expectedPositions = {Position(3, 4), Position(3, 5), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::RIGHT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_DOWN_RIGHT) {
    Position end(1, 5);
    std::vector<Position> expectedPositions = {Position(2, 4), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::DOWN_RIGHT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_DOWN) {
    Position end(1, 3);
    std::vector<Position> expectedPositions = {Position(2, 3), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::DOWN);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_DOWN_LEFT) {
    Position end(1, 1);
    std::vector<Position> expectedPositions = {Position(2, 2), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::DOWN_LEFT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, genCapturesInDirection_LEFT) {
    Position end(3, 1);
    std::vector<Position> expectedPositions = {Position(3, 2), end};

    piece._genCapturesInDirection(moves, end, Pieces::Move::Direction::LEFT);

    ASSERT_EQ(moves.size(), expectedPositions.size());
    for (const auto &expectedPosition : expectedPositions) {
        ASSERT_TRUE(moves.count(expectedPosition));
        ASSERT_TRUE(moves.at(expectedPosition).type() == Pieces::Move::Type::CAPTURE);
        ASSERT_EQ(moves.at(expectedPosition).actions().size(), 1);
        auto &expectedPiece = *moves.at(expectedPosition).actions()[0].piece();
        ASSERT_TRUE(expectedPiece == piece);
        ASSERT_TRUE(typeid(expectedPiece) == typeid(MockPiece1));
    }
}

TEST_F(PieceTest, IsInBounds_ValidPositions) {
    // Top-left corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(0, 0), 8, 8));
    // Bottom-right corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(7, 7), 8, 8));
    // Middle of the board
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(3, 4), 8, 8));
}

TEST_F(PieceTest, IsInBounds_InvalidPositions) {
    // Out of bounds (negative row)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(-1, 0), 8, 8));
    // Out of bounds (negative column)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(0, -1), 8, 8));
    // Out of bounds (exceeding limits)
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(8, 8), 8, 8));
    // Row exceeds board size
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(10, 5), 8, 8));
    // Column exceeds board size
    EXPECT_FALSE(Pieces::Piece::isInBounds(Position(3, 9), 8, 8));
}

TEST_F(PieceTest, EqualityOperators) {
    Pieces::Player player1("Alice");
    Pieces::Player player2("Bob");
    Position position1(1, 1);
    Position position2(2, 2);
    MockPiece1 piece1(position1);
    MockPiece1 piece2(position1);
    MockPiece1 piece3(position2);
    MockPiece1 piece4(position2);
    Pieces::Pawn piece5(position2);
    Pieces::Pawn piece6(position2);
    Pieces::Pawn piece7(position2, &player1);
    Pieces::Pawn piece8(position2, &player1);
    Pieces::Pawn piece9(position2, &player2);
    Pieces::Pawn piece10;
    Pieces::Pawn piece11;

    EXPECT_TRUE(piece1 == piece2);
    EXPECT_TRUE(piece2 != piece3);
    EXPECT_TRUE(piece3 == piece4);
    EXPECT_TRUE(piece4 != piece5);
    EXPECT_TRUE(piece5 == piece6);
    EXPECT_TRUE(piece6 != piece7);
    EXPECT_TRUE(piece7 == piece8);
    EXPECT_TRUE(piece8 != piece9);
    EXPECT_TRUE(piece9 != piece10);
    EXPECT_TRUE(piece10 == piece11);
}

TEST_F(PieceTest, Hash) {
    MockPiece1 piece1(Position(1, 1));
    MockPiece1 piece2(Position(1, 1));
    MockPiece1 piece3(Position(2, 2));
    MockPiece2 piece4(Position(2, 2));

    EXPECT_EQ(piece1.hash(), piece2.hash());
    EXPECT_NE(piece2.hash(), piece3.hash());
    EXPECT_EQ(piece3.hash(), piece4.hash());
}
