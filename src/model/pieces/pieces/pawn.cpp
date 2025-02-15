#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Pawn::Pawn()
        : Piece() {};
    Pawn::Pawn(const Position &position)
        : Piece(position) {};

    std::vector<Move> Pawn::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
