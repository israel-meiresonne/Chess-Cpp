#include <unordered_set>

#include "model/pieces/pieces.hpp"

namespace Pieces {
    Bishop::Bishop()
        : Piece() {};
    Bishop::Bishop(const Position &position)
        : Piece(position) {};

    std::unordered_set<Move> &Bishop::_moves(std::unordered_set<Move> &moves, int &nRow,
                                             int &nColumn) {
        bottomLeftDiagonalMoves(moves, nRow, nColumn);
        return bottomRightDiagonalMoves(moves, nRow, nColumn);
    };

    std::unordered_set<Move> &Bishop::bottomLeftDiagonalMoves(std::unordered_set<Move> &moves,
                                                              int &nRow, int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int minDiffToStart = std::min(initialRow, initialColumn);
        int startRow = initialRow - minDiffToStart;
        int startColumn = initialColumn - minDiffToStart;
        Position start(startRow, startColumn);

        int rowDiffToBound = nRow - initialRow - 1;
        int columnDiffToBound = nColumn - initialColumn - 1;
        int minDiffToEnd = std::min(rowDiffToBound, columnDiffToBound);
        int endRow = initialRow + minDiffToEnd;
        int endColumn = initialColumn + minDiffToEnd;
        Position end(endRow, endColumn);

        int rowDiff = 1, columnDiff = 1;
        this->genDirectionMoves(moves, start, end, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, start, end, rowDiff, columnDiff, Move::Type::CAPTURE);
    }

    std::unordered_set<Move> &Bishop::bottomRightDiagonalMoves(std::unordered_set<Move> &moves,
                                                               int &nRow, int &nColumn) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int rowDiffToOrigin = initialRow;
        int columnDiffToBound = nColumn - initialColumn - 1;
        int minDiffToStart = std::min(rowDiffToOrigin, columnDiffToBound);
        int startRow = initialRow - minDiffToStart;
        int startColumn = initialColumn + minDiffToStart;
        Position start(startRow, startColumn);

        int rowDiffToBound = nRow - initialRow - 1;
        int columnDiffToOrigin = initialColumn;
        int minDiffToEnd = std::min(rowDiffToBound, columnDiffToOrigin);
        int endRow = initialRow + minDiffToEnd;
        int endColumn = initialColumn - minDiffToEnd;
        Position end(endRow, endColumn);

        int rowDiff = 1, columnDiff = -1;
        this->genDirectionMoves(moves, start, end, rowDiff, columnDiff);
        return this->genDirectionMoves(moves, start, end, rowDiff, columnDiff, Move::Type::CAPTURE);
    }
} // namespace Pieces
