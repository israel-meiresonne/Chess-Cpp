#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Queen::Queen()
        : Piece() {};
    Queen::Queen(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> Queen::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
