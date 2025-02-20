#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Rook::Rook()
        : Piece() {};
    Rook::Rook(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Rook::_moves(std::unordered_set<Move> &moves, int &nRow,
                                           int &nColumn) {
        verticalMoves(moves, nRow);
        return horizontalMoves(moves, nColumn);
    };

    std::unordered_set<Move> &Rook::verticalMoves(std::unordered_set<Move> &moves, int &nRow) {
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

    std::unordered_set<Move> &Rook::horizontalMoves(std::unordered_set<Move> &moves, int &nColumn) {
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

} // namespace Pieces
