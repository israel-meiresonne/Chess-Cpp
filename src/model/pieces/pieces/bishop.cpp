#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Bishop::Bishop()
        : Piece() {};
    Bishop::Bishop(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Bishop::_moves(std::unordered_set<Move> &moves, int &nRow,
                                             int &nColumn) {
        bottomLeftDiagonalMoves(moves, nRow, nColumn);
        return bottomRightDiagonalMoves(moves, nRow, nColumn);
    };

} // namespace Pieces
