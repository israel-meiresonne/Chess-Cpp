#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Knight::Knight()
        : Piece() {};
    Knight::Knight(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> Knight::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
