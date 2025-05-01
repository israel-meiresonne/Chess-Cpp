#include <gtest/gtest.h>

#include <model/game/game.hpp>

class BoardTest : public ::testing::Test {
  protected:
    Game::Board board;
    Pieces::Player player1{"White"};
    Pieces::Player player2{"Black"};

    void SetUp() override {
        board = Game::Board(8, 8);
        board.initialize(player1, player2);
    }

    void clearBoard() {
        auto pieces = board.pieces();
        for (auto *piece : pieces) {
            piece->move(Position());
        }
    }

    static Pieces::Piece *findPiece(std::vector<Pieces::Piece *> &pieces,
                                    Pieces::Piece &targetPiece) {
        Pieces::Piece *piece = nullptr;

        for (auto *loopPiece : pieces) {
            if (*loopPiece != targetPiece) continue;

            piece = loopPiece;
            break;
        }
        if (piece == nullptr) {
            throw std::runtime_error("Piece not found: targetPiece='" + std::string(targetPiece) +
                                     "'");
        }
        return piece;
    }
};

TEST_F(BoardTest, ConstructorDefault) {
    Game::Board board;

    EXPECT_EQ(board.boundaries().first, 0);
    EXPECT_EQ(board.boundaries().second, 0);
    EXPECT_EQ(board.nMoves(), 0);
    EXPECT_EQ(board.moves().canUndo(), false);
    EXPECT_EQ(board.status(), Game::Status::NOT_STARTED);
    EXPECT_EQ(board.pieces().size(), 0);
}

TEST_F(BoardTest, ConstructorCustom) {
    board = Game::Board(8, 8);

    EXPECT_EQ(board.boundaries().first, 8);
    EXPECT_EQ(board.boundaries().second, 8);
    EXPECT_EQ(board.nMoves(), 0);
    EXPECT_EQ(board.moves().canUndo(), false);
    EXPECT_EQ(board.status(), Game::Status::NOT_STARTED);
    EXPECT_EQ(board.pieces().size(), 0);
}

TEST_F(BoardTest, Initialize) {
    auto pieces = board.pieces();
    std::vector<Pieces::Piece *> expectedPieces;

    // Ally pieces
    expectedPieces.push_back(new Pieces::Rook(Position(0, 0), &player1));
    expectedPieces.push_back(new Pieces::Knight(Position(0, 1), &player1));
    expectedPieces.push_back(new Pieces::Bishop(Position(0, 2), &player1));
    expectedPieces.push_back(new Pieces::Queen(Position(0, 3), &player1));
    expectedPieces.push_back(new Pieces::King(Position(0, 4), &player1));
    expectedPieces.push_back(new Pieces::Bishop(Position(0, 5), &player1));
    expectedPieces.push_back(new Pieces::Knight(Position(0, 6), &player1));
    expectedPieces.push_back(new Pieces::Rook(Position(0, 7), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 0), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 1), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 2), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 3), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 4), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 5), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 6), &player1));
    expectedPieces.push_back(new Pieces::Pawn(Position(1, 7), &player1));

    // Opponent Major Pieces (Row 7 or 0)
    expectedPieces.push_back(new Pieces::Rook(Position(7, 0), &player2));
    expectedPieces.push_back(new Pieces::Knight(Position(7, 1), &player2));
    expectedPieces.push_back(new Pieces::Bishop(Position(7, 2), &player2));
    expectedPieces.push_back(new Pieces::King(Position(7, 3), &player2));
    expectedPieces.push_back(new Pieces::Queen(Position(7, 4), &player2));
    expectedPieces.push_back(new Pieces::Bishop(Position(7, 5), &player2));
    expectedPieces.push_back(new Pieces::Knight(Position(7, 6), &player2));
    expectedPieces.push_back(new Pieces::Rook(Position(7, 7), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 0), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 1), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 2), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 3), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 4), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 5), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 6), &player2));
    expectedPieces.push_back(new Pieces::Pawn(Position(6, 7), &player2));

    EXPECT_EQ(board.status(), Game::Status::IN_PROGRESS);
    EXPECT_EQ(board.pieces().size(), 32);
    EXPECT_EQ(*board.pieces().front()->owner(), player1);
    EXPECT_EQ(*board.pieces().back()->owner(), player2);
    EXPECT_EQ(board.moves().canUndo(), false);
    for (auto *piece : expectedPieces) {
        EXPECT_EQ(*piece, *findPiece(pieces, *piece));
    }
    for (auto *piece : expectedPieces) {
        delete piece;
        piece = nullptr;
    }
}

TEST_F(BoardTest, Move) {
    auto pieces = board.pieces();
    Pieces::Piece *piece = nullptr;
    Position from(1, 2);
    Pieces::Pawn targetPiece(from, &player1);
    Position to1(3, 2);
    Position to2(4, 2);

    piece = findPiece(pieces, targetPiece);

    board.move(piece, to1);
    EXPECT_EQ(piece->position(), to1);
    EXPECT_EQ(piece->nMoves(), 1);
    EXPECT_EQ(board.moves().canUndo(), true);
    EXPECT_EQ(board.status(), Game::Status::IN_PROGRESS);

    board.move(piece, to2);
    EXPECT_EQ(piece->position(), to2);
    EXPECT_EQ(piece->nMoves(), 2);
    EXPECT_EQ(board.moves().canUndo(), true);
    EXPECT_EQ(board.status(), Game::Status::IN_PROGRESS);
}

TEST_F(BoardTest, MoveCapture) {
    auto pieces = board.pieces();
    Pieces::Piece *opponentQueen = new Pieces::Queen(Position(7, 4), &player2);
    Pieces::Piece *allyPawn = new Pieces::Pawn(Position(1, 4), &player1);
    Position toCapture(3, 3);
    Position captured;

    std::vector<Pieces::Piece *> toFreePieces{opponentQueen, allyPawn};

    opponentQueen = findPiece(pieces, *opponentQueen);
    allyPawn = findPiece(pieces, *allyPawn);

    opponentQueen->move(toCapture);
    allyPawn->move(Position(2, 4));

    board.move(allyPawn, Position(3, 3));
    EXPECT_EQ(board.status(), Game::Status::IN_PROGRESS);
    EXPECT_EQ(opponentQueen->position(), captured);
    EXPECT_EQ(allyPawn->position(), toCapture);

    for (auto *piece : toFreePieces) {
        delete piece;
        piece = nullptr;
    }
}

TEST_F(BoardTest, MoveWithWrongStatus) {
    board = Game::Board(8, 8);
    Pieces::Piece *piece = new Pieces::Pawn(Position(2, 2));

    EXPECT_THROW(board.move(piece, Position(1, 1)), std::runtime_error);
    delete piece;
}

TEST_F(BoardTest, MoveWithNotExistingPiece) {
    Pieces::Piece *piece = new Pieces::Pawn(Position(2, 2));

    EXPECT_THROW(board.move(piece, Position(1, 1)), std::runtime_error);
    delete piece;
}

TEST_F(BoardTest, MoveWithInvalidDestination) {
    auto pieces = board.pieces();
    Pieces::Pawn pawn(Position(1, 0), &player1);
    Pieces::Piece *piece = findPiece(pieces, pawn);

    EXPECT_THROW(board.move(piece, Position(2, 1)), std::runtime_error);
}

TEST_F(BoardTest, StatusIsCheckmate) {
    auto pieces = board.pieces();
    Pieces::Piece *opponentKing = new Pieces::King(Position(7, 3), &player2);
    Pieces::Piece *allyQueen = new Pieces::Queen(Position(0, 3), &player1);
    Pieces::Piece *allyRook1 = new Pieces::Rook(Position(0, 0), &player1);
    Pieces::Piece *allyRook2 = new Pieces::Rook(Position(0, 7), &player1);

    std::vector<Pieces::Piece *> toFreePieces{opponentKing, allyQueen, allyRook1, allyRook2};

    opponentKing = findPiece(pieces, *opponentKing);
    allyQueen = findPiece(pieces, *allyQueen);
    allyRook1 = findPiece(pieces, *allyRook1);
    allyRook2 = findPiece(pieces, *allyRook2);

    clearBoard();
    opponentKing->move(Position(3, 3));
    allyQueen->move(Position(4, 1));
    allyRook1->move(Position(4, 4));
    allyRook2->move(Position(2, 2));

    board.move(allyQueen, Position(4, 2));

    EXPECT_EQ(board.status(), Game::Status::ENDED_CHECKMATE);
    EXPECT_THROW(board.move(allyQueen, Position(3, 2)), std::runtime_error);

    for (auto *piece : toFreePieces) {
        delete piece;
        piece = nullptr;
    }
}

TEST_F(BoardTest, StatusIsStalemate) {
    auto pieces = board.pieces();
    Pieces::Piece *opponentKing = new Pieces::King(Position(7, 3), &player2);
    Pieces::Piece *allyQueen = new Pieces::Queen(Position(0, 3), &player1);
    Pieces::Piece *allyRook1 = new Pieces::Rook(Position(0, 0), &player1);
    Pieces::Piece *allyRook2 = new Pieces::Rook(Position(0, 7), &player1);

    std::vector<Pieces::Piece *> toFreePieces{opponentKing, allyQueen, allyRook1, allyRook2};

    opponentKing = findPiece(pieces, *opponentKing);
    allyQueen = findPiece(pieces, *allyQueen);
    allyRook1 = findPiece(pieces, *allyRook1);
    allyRook2 = findPiece(pieces, *allyRook2);

    clearBoard();
    opponentKing->move(Position(3, 3));
    allyQueen->move(Position(1, 5));
    allyRook1->move(Position(4, 2));
    allyRook2->move(Position(2, 2));

    board.move(allyQueen, Position(2, 5));
    EXPECT_EQ(board.status(), Game::Status::ENDED_STALEMATE);
    EXPECT_THROW(board.move(allyQueen, Position(3, 5)), std::runtime_error);

    for (auto *piece : toFreePieces) {
        delete piece;
        piece = nullptr;
    }
}

TEST_F(BoardTest, Serialize) {
    auto serialized = board.serialize();
    Pieces::Piece *piece00 = serialized[0][0];
    Pieces::Piece *piece77 = serialized[7][7];

    ASSERT_EQ(serialized.size(), board.boundaries().first);
    ASSERT_EQ(serialized[0].size(), board.boundaries().second);

    ASSERT_NE(piece00, nullptr);
    EXPECT_EQ(*piece00, Pieces::Rook(Position(0, 0), &player1));
    ASSERT_NE(piece77, nullptr);
    EXPECT_EQ(*piece77, Pieces::Rook(Position(7, 7), &player2));
    EXPECT_EQ(serialized[4][4], nullptr);
}
