#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Rook::Rook()
        : Piece() {};
    Rook::Rook(const Position &position)
        : Piece(position) {};

    std::vector<Move> Rook::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
