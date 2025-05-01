#ifndef GAME_HPP
#define GAME_HPP

#include <unordered_map>

#include "model/pieces/pieces.hpp"
#include "model/utils/templates.hpp"

namespace Game {
    class Status {
        enum class _Status {
            NOT_STARTED,
            IN_PROGRESS,
            ENDED_CHECKMATE,
            ENDED_STALEMATE,
            UNDEFINED = -1
        };

        Status();
        Status(_Status status);

        _Status _status;

      public:
        static const Status NOT_STARTED;
        static const Status IN_PROGRESS;
        static const Status ENDED_CHECKMATE;
        static const Status ENDED_STALEMATE;
        static const Status UNDEFINED;

        operator std::string() const;

        int hash() const;

        bool operator==(const Status &other) const;

        bool operator!=(const Status &other) const;
    };

    class Board {
      public:
        Board();
        Board(int nRow, int nColumn);
        ~Board();

        std::pair<int, int> boundaries() const;

        int nMoves() const;

        Status status() const;

        Utils::Templates::UndoRedo<Pieces::Move> moves() const;

        const std::vector<Pieces::Piece *> &pieces() const;

        void initialize(Pieces::Player &first, Pieces::Player &second);

        void move(Pieces::Piece *piece, Position to);

        void unMove();

        void reMove();

        std::unordered_map<Position, Pieces::Piece *> promotions(Pieces::Piece *piece);

        void promote(Pieces::Piece *piece, Pieces::Types promotion);

        std::vector<std::vector<Pieces::Piece *>> serialize() const;

      private:
        std::pair<int, int> _boundaries;
        int _nMoves;
        Status _status;
        Utils::Templates::UndoRedo<Pieces::Move> _moves;
        std::vector<Pieces::Piece *> _pieces;

        void updateStatus(Pieces::Player &player);

        bool opponentKingIsLastPiece(Pieces::Player &player);

        bool opponentKingHasMoves(Pieces::Player &player);

        bool isOpponentKingThreatened(Pieces::Player &player);

        std::vector<Pieces::Piece *> initializePieces(Pieces::Player &player, bool isFirstPlayer);

        bool pieceExists(Pieces::Piece *piece);

        std::unordered_map<Position, Pieces::Piece *>
        playerPieces(Pieces::Player &player, bool isFriendly, bool keepCaptured = true);

        std::unordered_map<Position, Pieces::Move> moves(Pieces::Piece *piece, Position &to);

        std::vector<Pieces::Action> actions(Pieces::Piece *piece, Position &to,
                                            std::unordered_map<Position, Pieces::Move> &moves);

        void playMove(Pieces::Player &player, std::vector<Pieces::Action> &actions);

        void unPlayMove();

        static Pieces::Piece *findKing(std::unordered_map<Position, Pieces::Piece *> pieces);

        static std::vector<Pieces::Piece *> findPieces(std::vector<Pieces::Piece *> pieces,
                                                       Pieces::Types type,
                                                       bool throwNotFound = false);
    };

    class Game {
        Pieces::Player *_player1;
        Pieces::Player *_player2;
        Pieces::Player *_currentPlayer;
        Board *_board;

        void nextPlayer();

        static Pieces::Piece *findPiece(std::vector<Pieces::Piece *> pieces, Position at,
                                        Pieces::Player *player);

      public:
        Game();
        Game(std::string player1, std::string player2);
        ~Game();

        Pieces::Player player1() const;
        Pieces::Player player2() const;
        Pieces::Player currentPlayer() const;
        std::vector<std::vector<Pieces::Piece *>> board() const;

        void start();
        Status status() const;
        std::vector<std::vector<Pieces::Piece *>> move(Position from, Position to);
    };
}; // namespace Game

#endif // GAME_HPP
