#include <vector>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Bishop::Bishop()
        : Piece() {};
    Bishop::Bishop(const Position &position)
        : Piece(position) {};

    std::vector<Move> Bishop::moves(int nRow, int nColumn) {
        std::vector<Move> moves;
        return moves;
    };
} // namespace Pieces
