#include "model/pieces/pieces.hpp"

namespace Pieces {
    Rook::Rook()
        : Piece(Types::ROOK) {};
    Rook::Rook(const Position &position)
        : Piece(position, Types::ROOK) {}
    Rook::Rook(const Position &position, Player *player)
        : Piece(position, player, Types::ROOK) {};

    std::unordered_map<Position, Move> &Rook::_moves(std::unordered_map<Position, Move> &moves,
                                                     int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};
        verticalMoves(moves, boundaries);
        horizontalMoves(moves, boundaries);
        return removeMovesOutsideBounds(moves, boundaries);
    };

} // namespace Pieces
