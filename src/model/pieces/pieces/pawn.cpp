#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Pawn::Pawn()
        : Piece() {};
    Pawn::Pawn(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> Pawn::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
