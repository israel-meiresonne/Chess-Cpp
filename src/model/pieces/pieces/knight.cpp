#include "model/pieces/pieces.hpp"

namespace Pieces {
    Knight::Knight()
        : Piece() {};
    Knight::Knight(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Knight::_moves(std::unordered_set<Move> &moves, int &nRow,
                                             int &nColumn) {
        return moves;
    };
} // namespace Pieces
