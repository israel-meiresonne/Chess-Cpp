#include "model/pieces/pieces.hpp"

namespace Pieces {

    Pawn::Pawn()
        : Piece() {}

    Pawn::Pawn(const Position &position)
        : Piece(position) {}

    std::unordered_set<Move> Pawn::moves(int nRow, int nColumn) {
        std::unordered_set<Move> moves;
        Position initialPosition = position();
        addCaptureMoves(initialPosition, nRow, nColumn, moves);

        if (nMoves() == 0) return addFirstMoves(initialPosition, nRow, nColumn, moves);

        return addNotFirstMoves(initialPosition, nRow, nColumn, moves);
    }

    std::unordered_set<Move> &Pawn::addNotFirstMoves(const Position &initialPosition, int nRow,
                                                     int nColumn, std::unordered_set<Move> &moves) {
        int newRow = initialPosition.row() + 1;
        int newColumn = initialPosition.column();
        Position newPosition(newRow, newColumn);
        if (!isInBounds(newPosition, nRow, nColumn)) return moves;

        Action action(this, initialPosition, newPosition);
        Move move;
        move.add(action);
        moves.insert(move);
        return moves;
    }

    std::unordered_set<Move> &Pawn::addCaptureMoves(const Position &initialPosition, int nRow,
                                                    int nColumn, std::unordered_set<Move> &moves) {
        int newRow = initialPosition.row() + 1;
        int column = initialPosition.column();
        std::vector<Position> capturePositions = {Position(newRow, column - 1),
                                                  Position(newRow, column + 1)};

        for (const Position &capture : capturePositions) {
            if (!isInBounds(capture, nRow, nColumn)) continue;

            Action action(this, initialPosition, capture);
            Move move(Move::Type::CAPTURE);
            move.add(action);
            moves.insert(move);
        }
        return moves;
    }

    std::unordered_set<Move> &Pawn::addFirstMoves(const Position &initialPosition, int nRow,
                                                  int nColumn, std::unordered_set<Move> &moves) {
        for (int n = 1; n <= 2; ++n) {
            int newRow = initialPosition.row() + n;
            int newColumn = initialPosition.column();
            Position newPosition(newRow, newColumn);

            if (!isInBounds(newPosition, nRow, nColumn)) break;

            Action action(this, initialPosition, newPosition);
            Move move;
            move.add(action);
            moves.insert(move);
        }
        return moves;
    }

} // namespace Pieces
