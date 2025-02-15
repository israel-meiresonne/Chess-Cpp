#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Queen::Queen()
        : Piece() {};
    Queen::Queen(const Position &position)
        : Piece(position) {};

    std::vector<Move> Queen::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
