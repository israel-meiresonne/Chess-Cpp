#include <gtest/gtest.h>

#include <model/game/game.hpp>

class GameTest : public ::testing::Test {
  protected:
    std::string namePlayer1 = "White";
    std::string namePlayer2 = "Black";
    Game::Game game = Game::Game();

    void SetUp() override { game.start(namePlayer1, namePlayer2); }

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

TEST_F(GameTest, ConstructorDefault) {
    Game::Game game;

    EXPECT_EQ(game.status(), Game::Status::NOT_STARTED);
    EXPECT_THROW(game.player1(), std::runtime_error);
    EXPECT_THROW(game.player2(), std::runtime_error);
    EXPECT_THROW(game.currentPlayer(), std::runtime_error);
    EXPECT_THROW(game.board(), std::runtime_error);
}

TEST_F(GameTest, ConstructorWithPlayer) {
    Game::Game game;

    EXPECT_EQ(game.status(), Game::Status::NOT_STARTED);
    EXPECT_THROW(game.player1(), std::runtime_error);
    EXPECT_THROW(game.player2(), std::runtime_error);
    EXPECT_THROW(game.currentPlayer(), std::runtime_error);
    EXPECT_THROW(game.board(), std::runtime_error);
}

TEST_F(GameTest, StartWhenGameHasAlreadyStarted) {
    EXPECT_THROW(game.start(namePlayer1, namePlayer2), std::runtime_error);
}

TEST_F(GameTest, Move) {
    Position player1From1(0, 1), player1To1(2, 0);
    Position player2From1(7, 6), player2To1(5, 7);

    Position player1From2(2, 0), player1To2(3, 2);
    Position player2From2(7, 1), player2To2(5, 2);

    // Move 1: Player 1
    EXPECT_EQ(game.currentPlayer().name(), namePlayer1);
    game.move(player1From1, player1To1);
    auto board = game.board();
    auto player1Knight = board[player1To1.row()][player1To1.column()];
    EXPECT_EQ(player1Knight->type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(*player1Knight->owner(), game.player1());
    EXPECT_EQ(game.currentPlayer().name(), namePlayer2);

    // Move 2 Player 2
    EXPECT_EQ(game.currentPlayer().name(), namePlayer2);
    game.move(player2From1, player2To1);
    board = game.board();
    auto player2Knight = board[player2To1.row()][player2To1.column()];
    EXPECT_EQ(player2Knight->type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(*player2Knight->owner(), game.player2());
    EXPECT_EQ(game.currentPlayer().name(), namePlayer1);

    // Move 3 Player 1
    EXPECT_EQ(game.currentPlayer().name(), namePlayer1);
    game.move(player1From2, player1To2);
    board = game.board();
    player1Knight = board[player1To2.row()][player1To2.column()];
    EXPECT_EQ(player1Knight->type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(*player1Knight->owner(), game.player1());
    EXPECT_EQ(game.currentPlayer().name(), namePlayer2);

    // Move 4 Player 2
    EXPECT_EQ(game.currentPlayer().name(), namePlayer2);
    game.move(player2From2, player2To2);
    board = game.board();
    player2Knight = board[player2To2.row()][player2To2.column()];
    EXPECT_EQ(player2Knight->type(), Pieces::Types::KNIGHT);
    EXPECT_EQ(*player2Knight->owner(), game.player2());
    EXPECT_EQ(game.currentPlayer().name(), namePlayer1);
}

TEST_F(GameTest, MoveWhenNoPieceAtFromPosition) {
    Position from(3, 3), to(4, 3);

    EXPECT_THROW(game.move(from, to), std::runtime_error);
}

TEST_F(GameTest, MoveOpponentPiece) {
    Position from(7, 6), to(5, 7);

    EXPECT_THROW(game.move(from, to), std::runtime_error);
}

TEST_F(GameTest, MoveWhenGameHasNotStarted) {
    Game::Game game;
    Position from(1, 1), to(2, 1);

    EXPECT_THROW(game.move(from, to), std::runtime_error);
}
