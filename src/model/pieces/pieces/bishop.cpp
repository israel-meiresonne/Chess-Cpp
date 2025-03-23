#include "model/pieces/pieces.hpp"

namespace Pieces {
    Bishop::Bishop()
        : Piece(Types::BISHOP) {};
    Bishop::Bishop(const Position &position)
        : Piece(position, Types::BISHOP) {}
    Bishop::Bishop(const Position &position, Player *player)
        : Piece(position, player, Types::BISHOP) {};

    std::unordered_map<Position, Move> &Bishop::_moves(std::unordered_map<Position, Move> &moves,
                                                       int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};
        downLeftDiagonalMoves(moves, boundaries);
        downRightDiagonalMoves(moves, boundaries);
        return removeMovesOutsideBounds(moves, boundaries);
    };

} // namespace Pieces
