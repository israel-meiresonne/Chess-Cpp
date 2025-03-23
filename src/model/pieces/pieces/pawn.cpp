#include "model/pieces/pieces.hpp"

namespace Pieces {
    Pawn::Pawn()
        : Piece(Types::PAWN) {}
    Pawn::Pawn(const Position &position)
        : Piece(position, Types::PAWN) {}
    Pawn::Pawn(const Position &position, Player *player)
        : Piece(position, player, Types::PAWN) {}

    std::unordered_map<Position, Move> &Pawn::_moves(std::unordered_map<Position, Move> &moves,
                                                     int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};
        return allMoves(moves, boundaries);
    }

    std::unordered_map<Position, Move> &Pawn::allMoves(std::unordered_map<Position, Move> &moves,
                                                       std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures = captureMoves(boundaries);
        int nRow = boundaries.first, nColumn = boundaries.second;

        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int maxDisplacement = (nMoves() == 0) ? 2 : 1;
        Position endDisplacement(initialRow + maxDisplacement, initialColumn);

        genMovesInDirection(moves, endDisplacement, Move::Direction::UP, captures);
        completeCaptureMoves(moves, captures);
        return removeMovesOutsideBounds(moves, boundaries);
    }

    std::unordered_map<Position, Move> Pawn::captureMoves(std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> moves;
        int nRow = boundaries.first, nColumn = boundaries.second;
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        std::vector<Position> capturePositions = {Position(initialRow + 1, initialColumn - 1),
                                                  Position(initialRow + 1, initialColumn + 1)};

        for (const Position &capturePosition : capturePositions) {
            if (!isInBounds(capturePosition, nRow, nColumn)) continue;

            moves[capturePosition] =
                Move::createMove(*this, initialPosition, capturePosition, Move::Type::CAPTURE);
        }
        return moves;
    }

    std::unordered_map<Position, Move> &
    Pawn::completeCaptureMoves(std::unordered_map<Position, Move> &moves,
                               std::unordered_map<Position, Move> &captures) {
        std::unordered_map<Position, Piece *> &opponents = this->opponents();
        for (auto &[finalPosition, capture] : captures) {
            if (!opponents.count(finalPosition)) continue;

            Piece *opponent = &*opponents.at(finalPosition);
            Position opponentPosition = opponent->position();
            Move::addAction(capture, &*opponent, opponentPosition);
            moves[opponentPosition] = capture;
        }
        return moves;
    }

} // namespace Pieces
