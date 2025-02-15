#ifndef PIECES_HPP
#define PIECES_HPP

#include <vector>

#include "model/player/player.hpp"
#include "model/position/position.hpp"

namespace Pieces {
    class Piece;

    class Action {
      private:
        Piece *_piece;
        Position _initial;
        Position _final;

      public:
        Action();
        Action(Piece *piece, const Position &initial, const Position &final);

        Piece *piece() const;
        Position initial() const;
        Position final() const;

        bool operator==(const Action &other) const;
    };

    class Move {
      private:
        std::vector<Action> _actions;
        Player _author;

      public:
        Move();
        Move(const Player &author);

        std::vector<Action> actions() const;
        Player author() const;

        void add(const Action &action);
    };

    class Piece {
      private:
        Position _position;
        int _nMoves;

      public:
        Piece();
        Piece(const Position &position);

        Position position() const;
        int nMoves() const;

        void move(const Position &position);
        virtual std::vector<Move> moves(int nRow, int nColumn) = 0;

        bool operator==(const Piece &other) const;
    };

    class King : public Piece {
      public:
        King();
        King(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };

    class Queen : public Piece {
      public:
        Queen();
        Queen(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };

    class Rook : public Piece {
      public:
        Rook();
        Rook(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };

    class Bishop : public Piece {
      public:
        Bishop();
        Bishop(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };

    class Knight : public Piece {
      public:
        Knight();
        Knight(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };

    class Pawn : public Piece {
      public:
        Pawn();
        Pawn(const Position &position);

        std::vector<Move> moves(int nRow, int nColumn) override;
    };
} // namespace Pieces
#endif // PIECES_HPP
