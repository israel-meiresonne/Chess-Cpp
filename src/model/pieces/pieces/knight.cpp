#include "model/pieces/pieces.hpp"

namespace Pieces {
    Knight::Knight()
        : Piece() {};
    Knight::Knight(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Knight::_moves(std::unordered_set<Move> &moves, int &nRow,
                                             int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        std::vector<Position> finalPositions = {
            Position(initialRow + 2, initialColumn - 1),
            Position(initialRow + 2, initialColumn + 1),
            Position(initialRow - 2, initialColumn - 1),
            Position(initialRow - 2, initialColumn + 1),

            Position(initialRow + 1, initialColumn + 2),
            Position(initialRow - 1, initialColumn + 2),
            Position(initialRow + 1, initialColumn - 2),
            Position(initialRow - 1, initialColumn - 2),
        };
        for (const auto &finalPosition : finalPositions) {
            if (!isInBounds(finalPosition, nRow, nColumn)) continue;

            Move displacementMove = Move::createMove(this, initialPosition, finalPosition);
            Move captureMove =
                Move::createMove(this, initialPosition, finalPosition, Move::Type::CAPTURE);
            moves.insert(displacementMove);
            moves.insert(captureMove);
        }
        return moves;
    };
} // namespace Pieces
