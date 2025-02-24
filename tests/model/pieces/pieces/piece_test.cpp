#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/position/position.hpp>

class MockPiece1 : public Pieces::Piece {
  public:
    MockPiece1()
        : Piece() {}
    MockPiece1(const Position &position)
        : Piece(position) {}

    std::unordered_map<Position, Pieces::Move> &
    _genDirectionMoves(std::unordered_map<Position, Pieces::Move> &moves, Position start,
                       Position end, int rowDiff, int columnDiff, Pieces::Move::Type moveType) {
        return genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);
    }
    std::unordered_map<Position, Pieces::Piece> &callFriendlies() { return friendlies(); };
    std::unordered_map<Position, Pieces::Piece> &callOpponents() { return opponents(); };

  protected:
    std::unordered_map<Position, Pieces::Move> &
    _moves(std::unordered_map<Position, Pieces::Move> &moves, int &nRow, int &nColumn) {
        return moves;
    }
};

class MockPiece2 : public MockPiece1 {
  public:
    MockPiece2(const Position &position)
        : MockPiece1(position) {}
};

class MockPiece3 : public MockPiece1 {
  public:
    MockPiece3(const Position &position)
        : MockPiece1(position) {}
};

TEST(PieceTest, DefaultConstructor) {
    MockPiece1 piece;
    EXPECT_EQ(typeid(piece.position()), typeid(Position));
    EXPECT_EQ(piece.nMoves(), 0);
}

TEST(PieceTest, PositionConstructor) {
    Position pos(2, 3);
    MockPiece1 piece(pos);
    EXPECT_EQ(piece.position(), pos);
    EXPECT_EQ(piece.nMoves(), 0);
}

TEST(PieceTest, Getter_Opponents) {
    Position position(2, 3);
    Position position2(3, 2);
    MockPiece3 piece(position);
    std::unordered_map<Position, Pieces::Piece> friendlies = {{position2, MockPiece3(position2)}};
    std::unordered_map<Position, Pieces::Piece> opponents = {{position, piece}};

    std::unordered_map<Position, Pieces::Move> moves = piece.moves(friendlies, 8, 8, opponents);

    EXPECT_THROW(piece.callOpponents(), std::runtime_error);
    EXPECT_EQ(moves.size(), 0);
}

TEST(PieceTest, Getter_Friendlies) {
    Position position(2, 3);
    Position position2(3, 2);
    MockPiece3 piece(position);
    std::unordered_map<Position, Pieces::Piece> friendlies = {{position2, MockPiece3(position2)}};
    std::unordered_map<Position, Pieces::Piece> opponents = {{position, piece}};

    std::unordered_map<Position, Pieces::Move> moves = piece.moves(friendlies, 8, 8, opponents);

    EXPECT_THROW(piece.callFriendlies(), std::runtime_error);
    EXPECT_EQ(moves.size(), 0);
}

TEST(PieceTest, Move_IncrementsMoveCount) {
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

TEST(PieceTest, GenMovesDirection_ThrowsWhenRowDiffAndColumnDiffAreZero) {
    Position start(0, 0);
    Position end(5, 5);
    MockPiece1 piece(Position(3, 3));
    std::unordered_map<Position, Pieces::Move> moves;
    int rowDiff = 0, columnDiff = 0;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    EXPECT_THROW(piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType),
                 std::runtime_error);
}

TEST(PieceTest, GenDirectionMoves_StartIsAtEnd) {
    Position start(3, 4);
    Position end(3, 4);
    MockPiece1 piece(Position(3, 2));
    Position piecePosition = piece.position();
    std::unordered_map<Position, Pieces::Move> moves;
    int rowDiff = 1, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    Pieces::Move::Type expectedMoveType = moveType;
    Position expectedMoveInitialPosition = piecePosition;
    Position expectedMoveFinalPosition = start;
    Pieces::Move expectedMove = Pieces::Move::createMove(
        piece, expectedMoveInitialPosition, expectedMoveFinalPosition, expectedMoveType);
    std::unordered_map<Position, Pieces::Move> expectedMoves = {
        {expectedMoveFinalPosition, expectedMove}};

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (auto &[expectedFinalPosition, expectedMove] : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedFinalPosition));
        EXPECT_EQ(moves[expectedFinalPosition], expectedMove);
    }
}

TEST(PieceTest, GenDirectionMoves_WhenDiffRowAndDiffColumnAreNotZero) {
    int startRow = 0, startColumn = 5;
    int endRow = 5, endColumn = 0;
    Position start(startRow, startColumn);
    Position end(endRow, endColumn);
    MockPiece1 piece(Position(3, 2));
    Position piecePosition = piece.position();
    std::unordered_map<Position, Pieces::Move> moves;
    int rowDiff = 1, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::unordered_map<Position, Pieces::Move> expectedMoves;
    std::vector<int> diffFactorFinalPositions = {0, 1, 2, 4, 5};
    for (const auto factor : diffFactorFinalPositions) {
        int finalRow = startRow + factor * rowDiff;
        int finalColumn = startColumn + factor * columnDiff;
        Position finalPosition = Position(finalRow, finalColumn);

        expectedMoves[finalPosition] = Pieces::Move::createMove(piece, piecePosition, finalPosition,
                                                                Pieces::Move::Type::CAPTURE);
    }

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (auto &[expectedFinalPosition, expectedMove] : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedFinalPosition));
        EXPECT_EQ(moves[expectedFinalPosition], expectedMove);
    }
}

TEST(PieceTest, GenDirectionMoves_DiffRowOnly) {
    int startRow = 5, startColumn = 3;
    int endRow = 0, endColumn = 3;
    Position start(startRow, startColumn);
    Position end(endRow, endColumn);
    MockPiece1 piece(Position(3, 3));
    Position piecePosition = piece.position();
    std::unordered_map<Position, Pieces::Move> moves;
    int rowDiff = -1, columnDiff = 0;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::unordered_map<Position, Pieces::Move> expectedMoves;
    std::vector<int> diffFactorFinalPositions = {0, 1, 3, 4, 5};
    for (const auto factor : diffFactorFinalPositions) {
        int finalRow = startRow + factor * rowDiff;
        int finalColumn = startColumn + factor * columnDiff;
        Position finalPosition = Position(finalRow, finalColumn);

        expectedMoves[finalPosition] = Pieces::Move::createMove(piece, piecePosition, finalPosition,
                                                                Pieces::Move::Type::CAPTURE);
    }

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (auto &[expectedFinalPosition, expectedMove] : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedFinalPosition));
        EXPECT_EQ(moves[expectedFinalPosition], expectedMove);
    }
}

TEST(PieceTest, GenDirectionMoves_DiffColumOnly) {
    int startRow = 3, startColumn = 5;
    int endRow = 3, endColumn = 0;
    Position start(startRow, startColumn);
    Position end(endRow, endColumn);
    MockPiece1 piece(Position(3, 3));
    Position piecePosition = piece.position();
    std::unordered_map<Position, Pieces::Move> moves;
    int rowDiff = 0, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::unordered_map<Position, Pieces::Move> expectedMoves;
    std::vector<int> diffFactorFinalPositions = {0, 1, 3, 4, 5};
    for (const auto factor : diffFactorFinalPositions) {
        int finalRow = startRow + factor * rowDiff;
        int finalColumn = startColumn + factor * columnDiff;
        Position finalPosition = Position(finalRow, finalColumn);

        expectedMoves[finalPosition] = Pieces::Move::createMove(piece, piecePosition, finalPosition,
                                                                Pieces::Move::Type::CAPTURE);
    }

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (auto &[expectedFinalPosition, expectedMove] : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedFinalPosition));
        EXPECT_EQ(moves[expectedFinalPosition], expectedMove);
    }
}

TEST(PieceTest, IsInBounds_ValidPositions) {
    // Top-left corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(0, 0), 8, 8));
    // Bottom-right corner
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(7, 7), 8, 8));
    // Middle of the board
    EXPECT_TRUE(Pieces::Piece::isInBounds(Position(3, 4), 8, 8));
}

TEST(PieceTest, IsInBounds_InvalidPositions) {
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

TEST(PieceTest, EqualityOperator) {
    MockPiece1 piece1(Position(1, 1));
    MockPiece1 piece2(Position(1, 1));
    MockPiece1 piece3(Position(2, 2));
    MockPiece2 piece4(Position(2, 2));

    EXPECT_TRUE(piece1 == piece2);
    EXPECT_FALSE(piece1 == piece3);
    EXPECT_FALSE(piece3 == piece4);
}

TEST(PieceTest, Hash) {
    MockPiece1 piece1(Position(1, 1));
    MockPiece1 piece2(Position(1, 1));
    MockPiece1 piece3(Position(2, 2));
    MockPiece2 piece4(Position(2, 2));

    EXPECT_EQ(piece1.hash(), piece2.hash());
    EXPECT_NE(piece1.hash(), piece3.hash());
    EXPECT_NE(piece3.hash(), piece4.hash());
}
