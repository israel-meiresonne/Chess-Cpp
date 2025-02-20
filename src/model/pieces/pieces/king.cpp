#include "model/pieces/pieces.hpp"

namespace Pieces {
    King::King()
        : Piece() {};
    King::King(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &King::_moves(std::unordered_set<Move> &moves, int &nRow,
                                           int &nColumn) {
        return moves;
    };
} // namespace Pieces
