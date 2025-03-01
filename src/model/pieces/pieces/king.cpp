#include "model/pieces/pieces.hpp"

namespace Pieces {
    King::King()
        : Piece(Types::KING) {};
    King::King(const Position &position)
        : Piece(position, Types::KING) {};

    std::unordered_map<Position, Move> &King::_moves(std::unordered_map<Position, Move> &moves,
                                                     int &nRow, int &nColumn) {
        std::pair<int, int> boundaries = {nRow, nColumn};

        castlingMoves(moves, boundaries);
        displacementAndCaptureMoves(moves);
        removeThreatenedMoves(moves, boundaries);
        return removeMovesOutsideBounds(moves, boundaries);
    }

    std::unordered_map<Position, Move> &
    King::removeThreatenedMoves(std::unordered_map<Position, Move> &moves,
                                std::pair<int, int> boundaries) {
        std::unordered_set<Position> threatenings = threateningPositions(boundaries);

        auto isThreat = [&threatenings](Position p) { return threatenings.count(p); };
        auto isSwapMove = [](Move m) { return m.type() == Move::Type::SWAP; };

        auto maxIteration = moves.size();
        auto i = maxIteration - maxIteration;
        auto it = moves.begin();
        while (it != moves.end()) {
            if (i == maxIteration) {
                throw std::runtime_error("Max iteraction reached: i=" + std::to_string(i));
            }

            Position position = it->first;
            Move move = it->second;
            if (isThreat(position)) {
                it = moves.erase(it);
                continue;
            }
            if (isSwapMove(move)) {
                Position rookFinalPosition = move.actions()[1].final();
                if (isThreat(rookFinalPosition)) {
                    it = moves.erase(it);
                    continue;
                }
            }
            it++;
            i++;
        }
        return moves;
    }

    std::unordered_map<Position, Move> &
    King::displacementAndCaptureMoves(std::unordered_map<Position, Move> &moves) {
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        std::vector<Position> finals = {
            Position(initialRow + 1, initialColumn),
            Position(initialRow - 1, initialColumn),
            Position(initialRow, initialColumn + 1),
            Position(initialRow, initialColumn - 1),
            Position(initialRow + 1, initialColumn + 1),
            Position(initialRow - 1, initialColumn - 1),
            Position(initialRow + 1, initialColumn - 1),
            Position(initialRow - 1, initialColumn + 1),
        };
        std::unordered_map<Position, Move> captures;
        Move::Direction direction;
        for (const auto &final : finals) {
            direction = Move::Direction(initialPosition, final);
            genCapturesInDirection(captures, final, direction);
        }
        for (const auto &final : finals) {
            direction = Move::Direction(initialPosition, final);
            genMovesInDirection(moves, final, direction, captures);
        }
        return moves;
    }

    std::unordered_map<Position, Move> &
    King::castlingMoves(std::unordered_map<Position, Move> &moves, std::pair<int, int> boundaries) {
        if (nMoves() != 0) return moves;

        const std::unordered_map<Position, Piece *> rooks = this->validRooks(boundaries);
        if (rooks.size() == 0) return moves;

        Position finalPosition;
        Position rookInitialPosition;
        Position rookFinalPosition;
        Position initialPosition = position();
        auto isKingCastling = [](Position &kingPosition, Position &rookPosition) {
            return (rookPosition.column() - kingPosition.column()) > 0;
        };
        for (auto &[position, rook] : rooks) {
            rookInitialPosition = rook->position();
            if (isKingCastling(initialPosition, rookInitialPosition)) {
                finalPosition = Position(0, 6);
                rookFinalPosition = Position(0, 5);
            } else {
                finalPosition = Position(0, 2);
                rookFinalPosition = Position(0, 3);
            }
            Move move = Move::createMove(*this, initialPosition, finalPosition, Move::Type::SWAP);
            Move::addAction(move, &*rook, rookInitialPosition, rookFinalPosition);
            moves[finalPosition] = move;
        }
        return moves;
    }

    std::unordered_map<Position, Piece *> King::validRooks(std::pair<int, int> boundaries) {
        std::unordered_map<Position, Piece *> &friendlies = this->friendlies();
        std::vector<Position> rookPositions = {Position(0, 0), Position(0, boundaries.second - 1)};
        std::unordered_map<Position, Piece *> rooks;

        for (auto &position : rookPositions) {
            if (!friendlies.count(position)) continue;

            Piece *piece = friendlies.at(position);
            if (piece->type() != Types::ROOK) continue;

            if (piece->nMoves() != 0) continue;

            if (!isPathToRookValid(boundaries, &*piece)) continue;

            rooks[position] = piece;
        }
        return rooks;
    }

    bool King::isPathToRookValid(std::pair<int, int> boundaries, const Piece *rook) {
        bool isPathToRookValid = false;
        std::unordered_map<Position, Piece *> &friendlies = this->friendlies();
        std::unordered_map<Position, Piece *> &opponents = this->opponents();

        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();
        Position rookPosition = rook->position();
        int rookColumn = rookPosition.column();

        if (initialRow != rookPosition.row()) return isPathToRookValid;

        int columnDiff = rookColumn - initialColumn;
        int columnUnitDiff =
            (columnDiff > 0) ? (columnDiff / columnDiff) : -(columnDiff / columnDiff);
        int column = initialColumn;
        Position positionToRook;
        while (!(positionToRook == rookPosition)) {
            positionToRook = Position(initialRow, column);
            column += columnUnitDiff;

            if (positionToRook == rookPosition) return true;

            if (friendlies.count(positionToRook) || opponents.count(positionToRook)) return false;

            if (!isInBounds(positionToRook, boundaries.first, boundaries.second)) {
                std::string positionStr = positionToRook;
                throw std::runtime_error("The position cannot be out of boundaries: position" +
                                         positionStr);
            }
        }
        return true;
    }

    std::unordered_set<Position> King::threateningPositions(std::pair<int, int> boundaries) {
        std::unordered_set<Position> threatenings;
        std::unordered_map<Position, Piece *> &friendlies = this->friendlies();
        std::unordered_map<Position, Piece *> &opponents = this->opponents();
        std::unordered_map<Position, Piece *> emptyOpponents;
        std::unordered_map<Position, Move> opponentMoves;
        for (auto &[position, opponent] : opponents) {
            opponentMoves =
                opponent->moves(emptyOpponents, boundaries.first, boundaries.second, friendlies);

            extractPositionsFromMoves(threatenings, opponentMoves);
        }
        return threatenings;
    };

    void King::extractPositionsFromMoves(std::unordered_set<Position> &threatenings,
                                         const std::unordered_map<Position, Move> &moves) {
        for (const auto &[position, move] : moves) {
            std::vector<Pieces::Action> actions = move.actions();
            if (actions.size() == 0) continue;

            threatenings.insert(move.actions()[0].final());
        }
    }

} // namespace Pieces
