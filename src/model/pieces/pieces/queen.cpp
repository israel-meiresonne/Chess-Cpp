#include "model/pieces/pieces.hpp"

namespace Pieces {
    Queen::Queen()
        : Piece(Types::QUEEN) {};
    Queen::Queen(const Position &position)
        : Piece(position, Types::QUEEN) {};

    std::unordered_map<Position, Move> &Queen::_moves(std::unordered_map<Position, Move> &moves,
                                                      int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};
        verticalMoves(moves, boundaries);
        horizontalMoves(moves, boundaries);
        downLeftDiagonalMoves(moves, boundaries);
        downRightDiagonalMoves(moves, boundaries);
        return removeMovesOutsideBounds(moves, boundaries);
    };
} // namespace Pieces
