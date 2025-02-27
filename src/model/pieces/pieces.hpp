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
        Piece(Types type = Types::UNDEFINED);

        Types type() const;
        Position position() const;
        int nMoves() const;

        void move(const Position position);
        std::unordered_map<Position, Move>
        moves(const std::unordered_map<Position, Piece> &friendlies, int nRow, int nColumn,
              const std::unordered_map<Position, Piece> &opponents);

        static bool isInBounds(const Position &position, int rowMaxBound, int columnMaxBound,
                               int rowMinBound = 0, int columnMinBound = 0);

        int hash() const;

        bool operator==(const Piece &other) const;

        operator std::string() const;

        friend std::ostream &operator<<(std::ostream &os, const Pieces::Piece &piece);

      protected:
        Piece(const Position position, Types type = Types::UNDEFINED);

        const std::unordered_map<Position, Piece> &friendlies();
        void friendlies(const std::unordered_map<Position, Piece> &friendlies);

        const std::unordered_map<Position, Piece> &opponents();
        void opponents(const std::unordered_map<Position, Piece> &opponents);

        virtual std::unordered_map<Position, Move> &
        _moves(std::unordered_map<Position, Move> &moves, int &nRow, int &nColumn);

        std::unordered_map<Position, Move> &
        removeMovesOutsideBounds(std::unordered_map<Position, Move> &moves,
                                 std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &verticalMoves(std::unordered_map<Position, Move> &moves,
                                                          std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &
        horizontalMoves(std::unordered_map<Position, Move> &moves, std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &
        downLeftDiagonalMoves(std::unordered_map<Position, Move> &moves,
                              std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &
        downRightDiagonalMoves(std::unordered_map<Position, Move> &moves,
                               std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &
        genMovesInDirection(std::unordered_map<Position, Move> &moves, Position end,
                            Move::Direction direction,
                            std::unordered_map<Position, Move> &captures);

        std::unordered_map<Position, Move> &
        genCapturesInDirection(std::unordered_map<Position, Move> &moves, Position end,
                               Move::Direction direction);

      private:
        Types _type;
        Position _position;
        int _nMoves;
        const std::unordered_map<Position, Piece> *_friendlies;
        const std::unordered_map<Position, Piece> *_opponents;
    };

    class King : public Piece {
      public:
        King();
        King(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;

      private:
        std::unordered_map<Position, Move> &
        removeThreatenedMoves(std::unordered_map<Position, Move> &moves,
                              std::pair<int, int> boundaries);

        std::unordered_map<Position, Move> &
        displacementAndCaptureMoves(std::unordered_map<Position, Move> &moves);

        std::unordered_map<Position, Move> &castlingMoves(std::unordered_map<Position, Move> &moves,
                                                          std::pair<int, int> boundaries);

        std::unordered_map<Position, const Piece *> validRooks(std::pair<int, int> boundaries);

        bool isPathToRookValid(std::pair<int, int> boundaries, const Piece &rook);

        std::unordered_set<Position> threateningPositions(std::pair<int, int> boundaries);

        void extractPositionsFromMoves(std::unordered_set<Position> &threatenings,
                                       const std::unordered_map<Position, Move> &moves);
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

      private:
        std::unordered_map<Position, Move> captureMoves(std::pair<int, int> boundaries);
        std::unordered_map<Position, Move> &allMoves(std::unordered_map<Position, Move> &moves,
                                                     std::pair<int, int> boundaries);
    };

    class Pawn : public Piece {
      public:
        Pawn();
        Pawn(const Position &position);

      protected:
        std::unordered_map<Position, Move> &_moves(std::unordered_map<Position, Move> &moves,
                                                   int &nRow, int &nColumn) override;

      private:
        std::unordered_map<Position, Move> &allMoves(std::unordered_map<Position, Move> &moves,
                                                     std::pair<int, int> boundaries);
        std::unordered_map<Position, Move> captureMoves(std::pair<int, int> boundaries);
        std::unordered_map<Position, Move> &
        completeCaptureMoves(std::unordered_map<Position, Move> &moves,
                             std::unordered_map<Position, Move> &captures);
    };

} // namespace Pieces

#endif // PIECES_HPP
