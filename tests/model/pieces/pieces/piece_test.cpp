#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/position/position.hpp>

class MockPiece1 : public Pieces::Piece {
  public:
    MockPiece1()
        : Piece() {}
    MockPiece1(const Position &position)
        : Piece(position) {}

    std::unordered_set<Pieces::Move> moves(int, int) override { return {}; }

    std::unordered_set<Pieces::Move> &_genDirectionMoves(std::unordered_set<Pieces::Move> &moves,
                                                         Position start, Position end, int rowDiff,
                                                         int columnDiff,
                                                         Pieces::Move::Type moveType) {
        return genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);
    }
};

class MockPiece2 : public MockPiece1 {
  public:
    MockPiece2(const Position &position)
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

TEST(PieceTest, MoveIncrementsMoveCount) {
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
    std::unordered_set<Pieces::Move> moves;
    int rowDiff = 0, columnDiff = 0;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    EXPECT_THROW(piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType),
                 std::runtime_error);
}

TEST(PieceTest, GenDirectionMoves_StartIsAtEnd) {
    Position start(3, 4);
    Position end(3, 4);
    MockPiece1 piece(Position(3, 2));
    std::unordered_set<Pieces::Move> moves;
    int rowDiff = 1, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::vector<Pieces::Move> expectedMoves = {Pieces::Move::createMove(
        &piece, piece.position(), Position(start.row(), start.column()), moveType)};

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}

TEST(PieceTest, GenDirectionMoves_DiffRowAndColumn) {
    Position start(0, 5);
    Position end(5, 0);
    MockPiece1 piece(Position(3, 2));
    std::unordered_set<Pieces::Move> moves;
    int rowDiff = 1, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::vector<Pieces::Move> expectedMoves = {
        Pieces::Move::createMove(&piece, piece.position(), Position(start.row(), start.column()),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(&piece, piece.position(),
                                 Position(start.row() + rowDiff, start.column() + columnDiff),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 2 * rowDiff, start.column() + 2 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 4 * rowDiff, start.column() + 4 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 5 * rowDiff, start.column() + 5 * columnDiff),
            Pieces::Move::Type::CAPTURE)};

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}

TEST(PieceTest, GenDirectionMoves_DiffRowOnly) {
    Position start(5, 3);
    Position end(0, 3);
    MockPiece1 piece(Position(3, 3));
    std::unordered_set<Pieces::Move> moves;
    int rowDiff = -1, columnDiff = 0;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::vector<Pieces::Move> expectedMoves = {
        Pieces::Move::createMove(&piece, piece.position(), Position(start.row(), start.column()),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(&piece, piece.position(),
                                 Position(start.row() + rowDiff, start.column() + columnDiff),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 3 * rowDiff, start.column() + 3 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 4 * rowDiff, start.column() + 4 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 5 * rowDiff, start.column() + 5 * columnDiff),
            Pieces::Move::Type::CAPTURE)};

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
    }
}

TEST(PieceTest, GenDirectionMoves_DiffColumOnly) {
    Position start(3, 5);
    Position end(3, 0);
    MockPiece1 piece(Position(3, 3));
    std::unordered_set<Pieces::Move> moves;
    int rowDiff = 0, columnDiff = -1;
    Pieces::Move::Type moveType = Pieces::Move::Type::CAPTURE;

    std::vector<Pieces::Move> expectedMoves = {
        Pieces::Move::createMove(&piece, piece.position(), Position(start.row(), start.column()),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(&piece, piece.position(),
                                 Position(start.row() + rowDiff, start.column() + columnDiff),
                                 Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 3 * rowDiff, start.column() + 3 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 4 * rowDiff, start.column() + 4 * columnDiff),
            Pieces::Move::Type::CAPTURE),
        Pieces::Move::createMove(
            &piece, piece.position(),
            Position(start.row() + 5 * rowDiff, start.column() + 5 * columnDiff),
            Pieces::Move::Type::CAPTURE)};

    piece._genDirectionMoves(moves, start, end, rowDiff, columnDiff, moveType);

    EXPECT_EQ(moves.size(), expectedMoves.size());
    for (const auto &expectedMove : expectedMoves) {
        EXPECT_TRUE(moves.count(expectedMove));
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
