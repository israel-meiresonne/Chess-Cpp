#include "model/game/game.hpp"

namespace Game {
    Game::Game()
        : _player1(nullptr)
        , _player2(nullptr)
        , _currentPlayer(nullptr)
        , _board(nullptr) {};

    Game::~Game() {
        if (this->_player1 != nullptr) delete this->_player1;
        if (this->_player2 != nullptr) delete this->_player2;
        if (this->_board != nullptr) delete this->_board;

        this->_player1 = nullptr;
        this->_player2 = nullptr;
        this->_currentPlayer = nullptr;
        this->_board = nullptr;
    }

    Pieces::Player Game::player1() const {
        if (this->_player1 == nullptr) throw std::runtime_error("Player1 is nullptr");
        return *this->_player1;
    }
    Pieces::Player Game::player2() const {
        if (this->_player2 == nullptr) throw std::runtime_error("Player2 is nullptr");
        return *this->_player2;
    }
    Pieces::Player Game::currentPlayer() const {
        if (this->_currentPlayer == nullptr) throw std::runtime_error("CurrentPlayer is nullptr");
        return *this->_currentPlayer;
    }

    std::vector<std::vector<Pieces::Piece *>> Game::board() const {
        if (this->_board == nullptr) throw std::runtime_error("Board is nullptr");
        return this->_board->serialize();
    }

    void Game::start(std::string &player1, std::string &player2) {
        if (this->_board) throw std::runtime_error("The game has already been started.");

        this->_player1 = new Pieces::Player(player1);
        this->_player2 = new Pieces::Player(player2);

        this->_board = new Board(8, 8);
        this->_board->initialize(*this->_player1, *this->_player2);
        this->nextPlayer();
    }

    Status Game::status() const {
        if (this->_board == nullptr) return Status::NOT_STARTED;

        return this->_board->status();
    }

    std::vector<std::vector<Pieces::Piece *>> Game::move(Position from, Position to) {
        if (!this->_board) throw std::runtime_error("The game has not started yet.");

        Pieces::Piece *piece = findPiece(this->_board->pieces(), from, this->_currentPlayer);
        this->_board->move(piece, to);

        this->nextPlayer();

        return this->_board->serialize();
    }

    void Game::nextPlayer() {
        this->_currentPlayer = (_currentPlayer == _player1) ? _player2 : _player1;
    }

    Pieces::Piece *Game::findPiece(std::vector<Pieces::Piece *> pieces, Position at,
                                   Pieces::Player *player) {
        Pieces::Piece *piece = nullptr;
        for (auto *loopPiece : pieces) {
            if ((loopPiece->position() != at) || (loopPiece->owner() != player)) continue;

            piece = loopPiece;
            break;
        }
        if (piece == nullptr) {
            throw std::runtime_error("Can't find a Piece owned by Player '" + std::string(*player) +
                                     "' at Position '" + std::string(at) + "'.");
        }
        return piece;
    }

} // namespace Game
