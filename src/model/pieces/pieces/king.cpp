#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    King::King()
        : Piece() {};
    King::King(const Position &position)
        : Piece(position) {};

    std::vector<Move> King::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
