#include "model/pieces/pieces.hpp"

namespace Pieces {
    Queen::Queen()
        : Piece() {};
    Queen::Queen(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Queen::_moves(std::unordered_set<Move> &moves, int &nRow,
                                            int &nColumn) {
        return moves;
    };
} // namespace Pieces
