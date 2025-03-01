#ifndef MOVE_HPP
#define MOVE_HPP

#include <utility>
#include <vector>

#include "model/position/position.hpp"
#include "model/utils/templates.hpp"

namespace Pieces {
    class Piece;

    class Action {
      public:
        Action();
        Action(const Piece *piece, const Position initial, const Position final);

        bool isPieceNullptr() const;
        const Piece *piece() const;
        Position initial() const;
        Position final() const;

        int hash() const;

        bool operator==(const Action &other) const;
        friend std::ostream &operator<<(std::ostream &os, const Pieces::Action &action);

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
        const Piece *_piece;
        Position _initial;
        Position _final;
    };

    class Move {
      public:
        enum class Type { DISPLACEMENT, CAPTURE, SWAP };
        friend std::ostream &operator<<(std::ostream &os, const Type &moveType);

        class Direction;

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

        static Move &addAction(Move &move, const Piece *piece, Position &initial,
                               Position final = Position());
    };

    class Move::Direction {
      public:
        enum class _Direction {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            UP_LEFT,
            UP_RIGHT,
            DOWN_LEFT,
            DOWN_RIGHT,
            CUSTOM,
            UNDEFINED = -1
        };

      public:
        static const Direction UP;
        static const Direction DOWN;
        static const Direction LEFT;
        static const Direction RIGHT;
        static const Direction UP_LEFT;
        static const Direction UP_RIGHT;
        static const Direction DOWN_LEFT;
        static const Direction DOWN_RIGHT;

        explicit Direction();
        explicit Direction(_Direction direction);
        explicit Direction(int rowDiff, int columnDiff);
        explicit Direction(Position initial, Position final);

        operator std::pair<int, int>() const;

      private:
        _Direction _direction;
        int _rowDiff;
        int _columnDiff;
    };

} // namespace Pieces

namespace std {
    template <>
    struct hash<Pieces::Move> {
        size_t operator()(const Pieces::Move &o) const { return Utils::Templates::hash(o); }
    };

} // namespace std

#endif // MOVE_HPP
