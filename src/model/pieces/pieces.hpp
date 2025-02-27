#ifndef PIECES_HPP
#define PIECES_HPP

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "model/pieces/move/move.hpp"
#include "model/position/position.hpp"
#include "model/utils/templates.hpp"

namespace Pieces {
    class Types {
      public:
        enum class _Types { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, UNDEFINED };

      public:
        static const Types KING;
        static const Types QUEEN;
        static const Types ROOK;
        static const Types BISHOP;
        static const Types KNIGHT;
        static const Types PAWN;
        static const Types UNDEFINED;

        explicit Types(_Types type = _Types::UNDEFINED);

        operator std::string() const;

        int hash() const;

        bool operator==(const Types &other) const;

      private:
        _Types _type;
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

#endif // PIECES_HPP
