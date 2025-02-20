#include "model/pieces/pieces.hpp"

namespace Pieces {
    Queen::Queen()
        : Piece() {};
    Queen::Queen(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Queen::_moves(std::unordered_set<Move> &moves, int &nRow,
                                            int &nColumn) {
        verticalMoves(moves, nRow);
        horizontalMoves(moves, nColumn);
        bottomLeftDiagonalMoves(moves, nRow, nColumn);
        return bottomRightDiagonalMoves(moves, nRow, nColumn);
    };
} // namespace Pieces
