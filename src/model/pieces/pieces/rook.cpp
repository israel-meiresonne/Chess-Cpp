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

} // namespace Pieces
