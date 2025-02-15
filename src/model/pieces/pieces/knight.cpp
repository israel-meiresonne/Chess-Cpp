#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Knight::Knight()
        : Piece() {};
    Knight::Knight(const Position &position)
        : Piece(position) {};

    std::vector<Move> Knight::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
