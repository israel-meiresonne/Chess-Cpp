#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Piece::Piece()
        : _position(Position())
        , _nMoves(0) {};
    Piece::Piece(const Position &position)
        : _position(position)
        , _nMoves(0) {};

    Position Piece::position() const { return _position; };
    int Piece::nMoves() const { return _nMoves; };

    void Piece::move(const Position &position) {
        _position = position;
        _nMoves++;
    };

    bool Piece::operator==(const Piece &other) const {
        return typeid(*this) == typeid(other) && _position == other._position &&
               _nMoves == other._nMoves;
    };
}; // namespace Pieces
