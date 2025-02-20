#include "model/pieces/pieces.hpp"

namespace Pieces {
    King::King()
        : Piece() {};
    King::King(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &King::_moves(std::unordered_set<Move> &moves, int &nRow,
                                           int &nColumn) {

        Position initial = position();
        int initialRow = initial.row();
        int initialColumn = initial.column();
        std::vector<Position> finals = {Position(initialRow + 1, initialColumn),
                                        Position(initialRow - 1, initialColumn),
                                        Position(initialRow, initialColumn + 1),
                                        Position(initialRow, initialColumn - 1),
                                        Position(initialRow + 1, initialColumn + 1),
                                        Position(initialRow - 1, initialColumn - 1),
                                        Position(initialRow + 1, initialColumn - 1),
                                        Position(initialRow - 1, initialColumn + 1)};

        std::unordered_set<Position> threatening = threateningPositions(nRow, nColumn);
        for (const auto &final : finals) {
            if (!isInBounds(final, nRow, nColumn) || threatening.count(final)) continue;

            Move displacement = Move::createMove(this, initial, final);
            Move capture = Move::createMove(this, initial, final, Move::Type::CAPTURE);
            moves.insert(displacement);
            moves.insert(capture);
        }
        return moves;
    };

    std::unordered_set<Position> King::threateningPositions(int nRow, int nColumn) {
        std::unordered_set<Position> threatening;
        for (auto *opponent : opponents()) {
            std::unordered_set<Move> moves = opponent->moves(nRow, nColumn);
            extractPositionsFromMoves(threatening, moves);
        }
        return threatening;
    };

    void King::extractPositionsFromMoves(std::unordered_set<Position> &threatening,
                                         std::unordered_set<Move> &moves) {
        for (const auto &move : moves) {
            for (const auto &action : move.actions()) {
                threatening.insert(action.final());
            }
        }
    }

} // namespace Pieces
