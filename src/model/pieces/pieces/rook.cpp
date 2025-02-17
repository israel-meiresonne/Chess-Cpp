#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Rook::Rook()
        : Piece() {};
    Rook::Rook(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> Rook::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
