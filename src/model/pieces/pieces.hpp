#ifndef PIECES_HPP
#define PIECES_HPP

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "model/player/player.hpp"
#include "model/position/position.hpp"
#include "model/utils/templates.hpp"

namespace Pieces {
    class Piece;

    class Action {
      private:
        Piece *_piece;
        Position _initial;
        Position _final;

      public:
        Action();
        Action(Piece *piece, const Position initial, const Position final);

        Piece *piece() const;
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
        Player _author;

      public:
        Move();
        Move(const Player &author, const Type &type = Move::Type::DISPLACEMENT);
        Move(const Type &type, const Player &author = Player());

        Type type() const;
        std::vector<Action> actions() const;
        Player author() const;

        void add(const Action &action);

        int hash() const;

        bool operator==(const Move &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Move &move);

        static Move createMove(Pieces::Piece *piece, Position initial, Position final,
                               Pieces::Move::Type moveType = Pieces::Move::Type::DISPLACEMENT);
    };

    class Piece {
      public:
        Piece();
        Piece(const Position position);

        Position position() const;
        int nMoves() const;

        void move(const Position position);
        std::unordered_set<Move> moves(int nRow, int nColumn, std::vector<Piece *> opponents = {});

        static bool isInBounds(const Position &position, int nRow, int nColumn);

        int hash() const;
        bool operator==(const Piece &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Piece &piece);

      protected:
        std::vector<Piece *> &opponents();

        virtual std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                                 int &nColumn) = 0;

        std::unordered_set<Move> &verticalMoves(std::unordered_set<Move> &moves, int &nRow);

        std::unordered_set<Move> &horizontalMoves(std::unordered_set<Move> &moves, int &nRow);

        std::unordered_set<Move> &bottomLeftDiagonalMoves(std::unordered_set<Move> &moves,
                                                          int &nRow, int &nColumn);

        std::unordered_set<Move> &bottomRightDiagonalMoves(std::unordered_set<Move> &moves,
                                                           int &nRow, int &nColumn);

        std::unordered_set<Move> &genDirectionMoves(std::unordered_set<Move> &moves, Position start,
                                                    Position end, int rowDiff, int columnDiff,
                                                    Move::Type moveType = Move::Type::DISPLACEMENT);

      private:
        Position _position;
        int _nMoves;
        std::vector<Piece *> _opponents;
    };

    class King : public Piece {
      public:
        King();
        King(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;

      private:
        std::unordered_set<Position> threateningPositions(int nRow, int nColumn);

        void extractPositionsFromMoves(std::unordered_set<Position> &threatening,
                                       std::unordered_set<Move> &moves);
    };

    class Queen : public Piece {
      public:
        Queen();
        Queen(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;
    };

    class Rook : public Piece {
      public:
        Rook();
        Rook(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;
    };

    class Bishop : public Piece {
      public:
        Bishop();
        Bishop(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;
    };

    class Knight : public Piece {
      public:
        Knight();
        Knight(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;
    };

    class Pawn : public Piece {
      public:
        Pawn();
        Pawn(const Position &position);

      protected:
        std::unordered_set<Move> &_moves(std::unordered_set<Move> &moves, int &nRow,
                                         int &nColumn) override;

      private:
        std::unordered_set<Move> &addFirstMoves(const Position &initialPosition, int nRow,
                                                int nColumn, std::unordered_set<Move> &moves);
        std::unordered_set<Move> &addNotFirstMoves(const Position &initialPosition, int nRow,
                                                   int nColumn, std::unordered_set<Move> &moves);
        std::unordered_set<Move> &addCaptureMoves(const Position &initialPosition, int nRow,
                                                  int nColumn, std::unordered_set<Move> &moves);
    };

} // namespace Pieces

namespace std {
    template <>
    struct hash<Pieces::Move> {
        size_t operator()(const Pieces::Move &o) const { return Utils::Templates::hash(o); }
    };

} // namespace std

#endif // PIECES_HPP
