#include "model/pieces/pieces.hpp"

namespace Pieces {
    Piece::Piece()
        : _position(Position())
        , _nMoves(0)
        , _friendlies(nullptr)
        , _opponents(nullptr) {};
    Piece::Piece(const Position position)
        : _position(position)
        , _nMoves(0)
        , _friendlies(nullptr)
        , _opponents(nullptr) {};

    Position Piece::position() const { return _position; };
    int Piece::nMoves() const { return _nMoves; };
    std::unordered_map<Position, Piece> &Piece::friendlies() {
        if (_friendlies == nullptr) throw std::runtime_error("Friendlies is nullptr");
        return *_friendlies;
    };
    std::unordered_map<Position, Piece> &Piece::opponents() {
        if (_friendlies == nullptr) throw std::runtime_error("Opponents is nullptr");
        return *_opponents;
    };

    void Piece::move(const Position position) {
        _position = position;
        _nMoves++;
    };

    std::unordered_map<Position, Move>
    Piece::moves(std::unordered_map<Position, Piece> &friendlies, int nRow, int nColumn,
                 std::unordered_map<Position, Piece> &opponents) {
        _friendlies = &friendlies;
        _opponents = &opponents;
        std::unordered_map<Position, Move> moves;
        if (!nRow && !nColumn) return moves;

        if (!isInBounds(position(), nRow, nColumn)) return moves;

        this->_moves(moves, nRow, nColumn);

        _friendlies = nullptr;
        _opponents = nullptr;
        return moves;
    };

    std::unordered_map<Position, Move> &Piece::_moves(std::unordered_map<Position, Move> &moves,
                                                      int &nRow, int &nColumn) {
        throw std::runtime_error("Child class must implement Piece::_moves method");
    }

    std::unordered_map<Position, Move> &
    Piece::verticalMoves(std::unordered_map<Position, Move> &moves, int &nRow) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        int endRow = nRow - 1;
        Position verticalStart(0, initialColumn);
        Position verticalEnd(endRow, initialColumn);

        int rowDiff = 1, columnDiff = 0;
        this->genDirectionMoves(moves, verticalStart, verticalEnd, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, verticalStart, verticalEnd, rowDiff, columnDiff,
                                       Move::Type::CAPTURE);
    }

    std::unordered_map<Position, Move> &
    Piece::horizontalMoves(std::unordered_map<Position, Move> &moves, int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        int endColumn = nColumn - 1;
        Position horizontalStart(initialRow, 0);
        Position horizontalEnd(initialRow, endColumn);

        int rowDiff = 0, columnDiff = 1;
        this->genDirectionMoves(moves, horizontalStart, horizontalEnd, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, horizontalStart, horizontalEnd, rowDiff, columnDiff,
                                       Move::Type::CAPTURE);
    }

    std::unordered_map<Position, Move> &
    Piece::bottomLeftDiagonalMoves(std::unordered_map<Position, Move> &moves, int &nRow,
                                   int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int minDiffToStart = std::min(initialRow, initialColumn);
        int startRow = initialRow - minDiffToStart;
        int startColumn = initialColumn - minDiffToStart;
        Position start(startRow, startColumn);

        int rowDiffToBound = nRow - initialRow - 1;
        int columnDiffToBound = nColumn - initialColumn - 1;
        int minDiffToEnd = std::min(rowDiffToBound, columnDiffToBound);
        int endRow = initialRow + minDiffToEnd;
        int endColumn = initialColumn + minDiffToEnd;
        Position end(endRow, endColumn);

        int rowDiff = 1, columnDiff = 1;
        this->genDirectionMoves(moves, start, end, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, start, end, rowDiff, columnDiff, Move::Type::CAPTURE);
    }

    std::unordered_map<Position, Move> &
    Piece::bottomRightDiagonalMoves(std::unordered_map<Position, Move> &moves, int &nRow,
                                    int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int rowDiffToOrigin = initialRow;
        int columnDiffToBound = nColumn - initialColumn - 1;
        int minDiffToStart = std::min(rowDiffToOrigin, columnDiffToBound);
        int startRow = initialRow - minDiffToStart;
        int startColumn = initialColumn + minDiffToStart;
        Position start(startRow, startColumn);

        int rowDiffToBound = nRow - initialRow - 1;
        int columnDiffToOrigin = initialColumn;
        int minDiffToEnd = std::min(rowDiffToBound, columnDiffToOrigin);
        int endRow = initialRow + minDiffToEnd;
        int endColumn = initialColumn - minDiffToEnd;
        Position end(endRow, endColumn);

        int rowDiff = 1, columnDiff = -1;
        this->genDirectionMoves(moves, start, end, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, start, end, rowDiff, columnDiff, Move::Type::CAPTURE);
    }

    std::unordered_map<Position, Move> &
    Piece::genDirectionMoves(std::unordered_map<Position, Move> &moves, Position start,
                             Position end, int rowDiff, int columnDiff, Move::Type moveType) {
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

            Move move = Move::createMove(*this, initialPosition, finalPosition, moveType);
            moves[finalPosition] = move;
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
