#include "model/pieces/pieces.hpp"

namespace Pieces {
    Knight::Knight()
        : Piece(Types::KNIGHT) {};
    Knight::Knight(const Position &position)
        : Piece(position, Types::KNIGHT) {}
    Knight::Knight(const Position &position, Player *player)
        : Piece(position, player, Types::KNIGHT) {};

    std::unordered_map<Position, Move> &Knight::_moves(std::unordered_map<Position, Move> &moves,
                                                       int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};
        return allMoves(moves, boundaries);
    };

    std::unordered_map<Position, Move> &Knight::allMoves(std::unordered_map<Position, Move> &moves,
                                                         std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures = captureMoves(boundaries);
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        Move::Direction direction;
        for (auto &[position, capture] : captures) {
            direction = Move::Direction(initialPosition, position);
            genMovesInDirection(moves, position, direction, captures);
        }
        return moves;
    }

    std::unordered_map<Position, Move> Knight::captureMoves(std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures;
        int nRow = boundaries.first, nColumn = boundaries.second;
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        std::vector<Position> capturedPositions = {
            Position(initialRow + 2, initialColumn - 1),
            Position(initialRow + 2, initialColumn + 1),
            Position(initialRow - 2, initialColumn - 1),
            Position(initialRow - 2, initialColumn + 1),

            Position(initialRow + 1, initialColumn + 2),
            Position(initialRow - 1, initialColumn + 2),
            Position(initialRow + 1, initialColumn - 2),
            Position(initialRow - 1, initialColumn - 2),
        };
        for (const auto &capturedPosition : capturedPositions) {
            if (!isInBounds(capturedPosition, nRow, nColumn)) continue;

            captures[capturedPosition] =
                Move::createMove(*this, initialPosition, capturedPosition, Move::Type::CAPTURE);
        }
        return captures;
    };

} // namespace Pieces
