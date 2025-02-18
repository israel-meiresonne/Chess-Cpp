#ifndef PIECES_HPP
#define PIECES_HPP

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
    };

    class Piece {
      private:
        Position _position;
        int _nMoves;

      public:
        Piece();
        Piece(const Position position);

        Position position() const;
        int nMoves() const;

        void move(const Position position);
        virtual std::unordered_set<Move> moves(int nRow, int nColumn) = 0;

        int hash() const;

        bool operator==(const Piece &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Piece &piece);
    };

    class King : public Piece {
      public:
        King();
        King(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;
    };

    class Queen : public Piece {
      public:
        Queen();
        Queen(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;
    };

    class Rook : public Piece {
      public:
        Rook();
        Rook(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;
    };

    class Bishop : public Piece {
      public:
        Bishop();
        Bishop(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;
    };

    class Knight : public Piece {
      public:
        Knight();
        Knight(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;
    };

    class Pawn : public Piece {
      public:
        Pawn();
        Pawn(const Position &position);

        std::unordered_set<Move> moves(int nRow, int nColumn) override;

        static bool isInBounds(const Position &position, int nRow, int nColumn);

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
