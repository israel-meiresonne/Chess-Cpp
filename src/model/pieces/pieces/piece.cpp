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

    std::unordered_set<Move> &Piece::genDirectionMoves(std::unordered_set<Move> &moves,
                                                       Position start, Position end, int rowDiff,
                                                       int columnDiff, Move::Type moveType) {
        if ((rowDiff == 0) && (columnDiff == 0)) {
            throw std::runtime_error("rowDiff and columnDiff can't both equal zero");
        }

        Position initialPosition = position();
        int startRow = start.row(), row = startRow;
        int startColumn = start.column(), column = startColumn;
        int endRow = end.row();
        int endColumn = end.column();

        int rowMinBound = std::min(startRow, endRow);
        int rowMaxBound = std::max(startRow, endRow);
        int columnMinBound = std::min(startColumn, endColumn);
        int columnMaxBound = std::max(startColumn, endColumn);

        while (row >= rowMinBound && row <= rowMaxBound && column >= columnMinBound &&
               column <= columnMaxBound) {
            Position finalPosition(row, column);
            row += rowDiff, column += columnDiff;
            if (finalPosition == initialPosition) continue;

            Move move = Move::createMove(this, initialPosition, finalPosition, moveType);
            moves.insert(move);
        }
        return moves;
    }

    bool Piece::isInBounds(const Position &position, int nRow, int nColumn) {
        return position.row() >= 0 && position.row() < nRow && position.column() >= 0 &&
               position.column() < nColumn;
    }

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

}; // namespace Pieces
