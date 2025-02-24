#ifndef PIECES_HPP
#define PIECES_HPP

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "model/position/position.hpp"
#include "model/utils/templates.hpp"

namespace Pieces {
    class Piece;

    class Action {
        /**
         * @brief The piece that is being moved
         * @warning
         *    - This pointer must Point to the original Piece store at
         *      &Player.pieces()[position] because Player is the owner/source
         *      of the Piece.
         *    - If by any mistake it points to an intermediary this pointer
         *      will dangle because all intermediaries will be destroy at some
         *      point.
         */
        Piece *_piece;
        Position _initial;
        Position _final;

      public:
        Action();
        Action(Piece *piece, const Position initial, const Position final);

        bool isPieceNullptr() const;
        const Piece &piece() const;
        Position initial() const;
        Position final() const;

        int hash() const;

        bool operator==(const Action &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Action &action);
    };

    class Move {
      public:
        enum class Type { DISPLACEMENT, CAPTURE, SWAP };

        friend std::ostream &operator<<(std::ostream &os, const Type &moveType);

      private:
        Type _type;
        std::vector<Action> _actions;

      public:
        Move();
        Move(const Type type);

        Type type() const;
        std::vector<Action> actions() const;

        void add(const Action &action);

        int hash() const;

        bool operator==(const Move &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Move &move);

        static Move createMove(Pieces::Piece &piece, Position initial, Position final,
                               Pieces::Move::Type moveType = Pieces::Move::Type::DISPLACEMENT);
    };

    class Piece {
      public:
        Piece();

        Position position() const;
        int nMoves() const;

        void move(const Position position);
        std::unordered_map<Position, Move> moves(std::unordered_map<Position, Piece> &friendlies,
                                                 int nRow, int nColumn,
                                                 std::unordered_map<Position, Piece> &opponents);

        static bool isInBounds(const Position &position, int nRow, int nColumn);

        int hash() const;
        bool operator==(const Piece &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Piece &piece);

      protected:
        Piece(const Position position);

        std::unordered_map<Position, Piece> &friendlies();
        std::unordered_map<Position, Piece> &opponents();

        virtual std::unordered_map<Position, Move> &
        _moves(std::unordered_map<Position, Move> &moves, int &nRow, int &nColumn);

        std::unordered_map<Position, Move> &verticalMoves(std::unordered_map<Position, Move> &moves,
                                                          int &nRow);

        std::unordered_map<Position, Move> &
        horizontalMoves(std::unordered_map<Position, Move> &moves, int &nRow);

        std::unordered_map<Position, Move> &
        bottomLeftDiagonalMoves(std::unordered_map<Position, Move> &moves, int &nRow, int &nColumn);

        std::unordered_map<Position, Move> &
        bottomRightDiagonalMoves(std::unordered_map<Position, Move> &moves, int &nRow,
                                 int &nColumn);

        std::unordered_map<Position, Move> &
        genDirectionMoves(std::unordered_map<Position, Move> &moves, Position start, Position end,
                          int rowDiff, int columnDiff,
                          Move::Type moveType = Move::Type::DISPLACEMENT);

      private:
        Position _position;
        int _nMoves;
        std::unordered_map<Position, Piece> *_friendlies;
        std::unordered_map<Position, Piece> *_opponents;
    };

    class King : public Piece {
      public:
        King();
        King(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;

      private:
        std::unordered_set<Position> threateningPositions(int nRow, int nColumn);

        void extractPositionsFromMoves(std::unordered_set<Position> &threatening,
                                       std::unordered_map<Position, Move> &moves);
    };

    class Queen : public Piece {
      public:
        Queen();
        Queen(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;
    };

    class Rook : public Piece {
      public:
        Rook();
        Rook(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;
    };

    class Bishop : public Piece {
      public:
        Bishop();
        Bishop(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;
    };

    class Knight : public Piece {
      public:
        Knight();
        Knight(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;
    };

    class Pawn : public Piece {
      public:
        Pawn();
        Pawn(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;

      private:
        std::unordered_map<Position, Move> firstMoves(int nRow, int nColumn);
        std::unordered_map<Position, Move> notFirstMoves(int nRow, int nColumn);
        std::unordered_map<Position, Move> captureMoves(int nRow, int nColumn);
    };

} // namespace Pieces

namespace std {
    template <>
    struct hash<Pieces::Move> {
        size_t operator()(const Pieces::Move &o) const { return Utils::Templates::hash(o); }
    };

} // namespace std

#endif // PIECES_HPP
