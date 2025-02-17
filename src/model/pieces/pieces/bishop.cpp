#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Bishop::Bishop()
        : Piece() {};
    Bishop::Bishop(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> Bishop::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        return moves;
    };
} // namespace Pieces
