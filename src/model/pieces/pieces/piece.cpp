#include "model/pieces/pieces.hpp"

namespace Pieces {
    Piece::Piece()
        : _position(Position())
        , _nMoves(0) {};
    Piece::Piece(const Position position)
        : _position(position)
        , _nMoves(0) {};

    Position Piece::position() const { return _position; };
    int Piece::nMoves() const { return _nMoves; };

    void Piece::move(const Position position) {
        _position = position;
        _nMoves++;
    };

    int Piece::hash() const {
        return typeid(*this).hash_code() ^ (_position.hash() << 1) ^ (_nMoves << 2);
    };

    bool Piece::operator==(const Piece &other) const {
        return typeid(*this) == typeid(other) && _position == other._position &&
               _nMoves == other._nMoves;
    };

    std::ostream &operator<<(std::ostream &os, const Pieces::Piece &piece) {
        os << "Piece(" << typeid(piece).name() << ", " << piece.position() << ")";
        return os;
    }

    bool Piece::isInBounds(const Position &position, int nRow, int nColumn) {
        return position.row() >= 0 && position.row() < nRow && position.column() >= 0 &&
               position.column() < nColumn;
    }

}; // namespace Pieces
