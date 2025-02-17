#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    King::King()
        : Piece() {};
    King::King(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> King::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
