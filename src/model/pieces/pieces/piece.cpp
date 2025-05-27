#include "model/pieces/pieces.hpp"

namespace Pieces {
    Piece::Piece(Types type)
        : _type(type)
        , _position(Position())
        , _nMoves(0)
        , _owner(nullptr)
        , _friendlies(nullptr)
        , _opponents(nullptr) {};
    Piece::Piece(const Position position, Types type)
        : _type(type)
        , _position(position)
        , _nMoves(0)
        , _owner(nullptr)
        , _friendlies(nullptr)
        , _opponents(nullptr) {};
    Piece::Piece(const Position position, Player *player, Types type)
        : _type(type)
        , _position(position)
        , _nMoves(0)
        , _owner(player)
        , _friendlies(nullptr)
        , _opponents(nullptr) {};

    Types Piece::type() const { return _type; }
    Position Piece::position() const { return _position; };
    int Piece::nMoves() const { return _nMoves; };

    bool Piece::isPlayerNullptr() const { return _owner == nullptr; }

    Player *Piece::owner() const {
        if (this->isPlayerNullptr()) throw std::runtime_error("Player is nullptr");
        return _owner;
    };

    std::unordered_map<Position, Piece *> &Piece::friendlies() {
        if (_friendlies == nullptr) throw std::runtime_error("Friendlies is nullptr");
        return *_friendlies;
    };
    void Piece::friendlies(std::unordered_map<Position, Piece *> &friendlies) {
        _friendlies = &friendlies;
    };

    std::unordered_map<Position, Piece *> &Piece::opponents() {
        if (_friendlies == nullptr) throw std::runtime_error("Opponents is nullptr");
        return *_opponents;
    };
    void Piece::opponents(std::unordered_map<Position, Piece *> &opponents) {
        _opponents = &opponents;
    };

    void Piece::move(const Position position) {
        _position = position;
        _nMoves++;
    };

    std::unordered_map<Position, Move>
    Piece::moves(std::unordered_map<Position, Piece *> &friendlies, int nRow, int nColumn,
                 std::unordered_map<Position, Piece *> &opponents) {
        _friendlies = &friendlies;
        _opponents = &opponents;
        std::unordered_map<Position, Move> moves;
        if (!nRow && !nColumn) return moves;

        if (!isInBounds(position(), nRow, nColumn)) return moves;

        this->_moves(moves, nRow, nColumn);
        _friendlies = nullptr;
        _opponents = nullptr;
        return moves;
    };

    std::unordered_map<Position, Move> &Piece::_moves(std::unordered_map<Position, Move> &moves,
                                                      int &nRow, int &nColumn) {
        throw std::runtime_error("Child class must implement method 'Piece::_moves'");
    }

    std::string Piece::icon() const {
        auto pieceType = this->_type;
        if (pieceType == Types::KING) return "♔";
        if (pieceType == Types::QUEEN) return "♕";
        if (pieceType == Types::ROOK) return "♖";
        if (pieceType == Types::BISHOP) return "♗";
        if (pieceType == Types::KNIGHT) return "♘";
        if (pieceType == Types::PAWN) return "♙";

        throw std::runtime_error("Unsupported Piece::Types: '" + std::string(pieceType) + "'");
    }

    std::unordered_map<Position, Move> &
    Piece::removeMovesOutsideBounds(std::unordered_map<Position, Move> &moves,
                                    std::pair<int, int> boundaries) {
        int nRow = boundaries.first, nColumn = boundaries.second;
        for (auto it = moves.begin(); it != moves.end();) {
            const Position &finalPosition = it->first;
            Move &move = it->second;
            if (!isInBounds(finalPosition, nRow, nColumn)) {
                it = moves.erase(it);
                continue;
            }
            ++it;
        }
        return moves;
    }

    std::unordered_map<Position, Move> &
    Piece::verticalMoves(std::unordered_map<Position, Move> &moves,
                         std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures;
        int nRow = boundaries.first, nColumn = boundaries.second;

        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        Position endUp(nRow - 1, initialColumn);
        Position endDown(0, initialColumn);
        genCapturesInDirection(captures, endUp, Move::Direction::UP);
        genCapturesInDirection(captures, endDown, Move::Direction::DOWN);
        genMovesInDirection(moves, endUp, Move::Direction::UP, captures);
        return genMovesInDirection(moves, endDown, Move::Direction::DOWN, captures);
    }

    std::unordered_map<Position, Move> &
    Piece::horizontalMoves(std::unordered_map<Position, Move> &moves,
                           std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures;
        int nRow = boundaries.first, nColumn = boundaries.second;

        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        Position endLeft(initialRow, 0);
        Position endDown(initialRow, nColumn - 1);
        genCapturesInDirection(captures, endLeft, Move::Direction::LEFT);
        genCapturesInDirection(captures, endDown, Move::Direction::RIGHT);
        genMovesInDirection(moves, endLeft, Move::Direction::LEFT, captures);
        return genMovesInDirection(moves, endDown, Move::Direction::RIGHT, captures);
    }

    std::unordered_map<Position, Move> &
    Piece::downLeftDiagonalMoves(std::unordered_map<Position, Move> &moves,
                                 std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures;
        int nRow = boundaries.first, nColumn = boundaries.second;

        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int minDiffToDownLeft = std::min(initialRow, initialColumn);
        int downleftEndRow = initialRow - minDiffToDownLeft;
        int downleftEndColumn = initialColumn - minDiffToDownLeft;
        Position downLeftEnd(downleftEndRow, downleftEndColumn);

        int rowDiffToUpBound = nRow - initialRow - 1;
        int columnDiffToRightBound = nColumn - initialColumn - 1;
        int minDiffToUpRightBounds = std::min(rowDiffToUpBound, columnDiffToRightBound);
        int upRightEndRow = initialRow + minDiffToUpRightBounds;
        int upRightEndColumn = initialColumn + minDiffToUpRightBounds;
        Position upRightEnd(upRightEndRow, upRightEndColumn);

        genCapturesInDirection(captures, downLeftEnd, Move::Direction::DOWN_LEFT);
        genCapturesInDirection(captures, upRightEnd, Move::Direction::UP_RIGHT);
        genMovesInDirection(moves, downLeftEnd, Move::Direction::DOWN_LEFT, captures);
        return genMovesInDirection(moves, upRightEnd, Move::Direction::UP_RIGHT, captures);
    }

    std::unordered_map<Position, Move> &
    Piece::downRightDiagonalMoves(std::unordered_map<Position, Move> &moves,
                                  std::pair<int, int> boundaries) {
        std::unordered_map<Position, Move> captures;
        int nRow = boundaries.first, nColumn = boundaries.second;
        Position initialPosition = position();
        int initialRow = initialPosition.row();
        int initialColumn = initialPosition.column();

        int rowDiffToDownBound = initialRow;
        int columnDiffToRightBound = nColumn - initialColumn - 1;
        int minDiffToDownRightBounds = std::min(rowDiffToDownBound, columnDiffToRightBound);
        int downRightRow = initialRow - minDiffToDownRightBounds;
        int downRightColumn = initialColumn + minDiffToDownRightBounds;
        Position downRightEnd(downRightRow, downRightColumn);

        int rowDiffToUpBound = nRow - initialRow - 1;
        int columnDiffToLeftBound = initialColumn;
        int minDiffToUpRightBounds = std::min(rowDiffToUpBound, columnDiffToLeftBound);
        int upLeftRow = initialRow + minDiffToUpRightBounds;
        int upLeftColumn = initialColumn - minDiffToUpRightBounds;
        Position upLeftEnd(upLeftRow, upLeftColumn);

        genCapturesInDirection(captures, downRightEnd, Move::Direction::DOWN_RIGHT);
        genCapturesInDirection(captures, upLeftEnd, Move::Direction::UP_LEFT);
        genMovesInDirection(moves, downRightEnd, Move::Direction::DOWN_RIGHT, captures);
        return genMovesInDirection(moves, upLeftEnd, Move::Direction::UP_LEFT, captures);
    }

    std::unordered_map<Position, Move> &
    Piece::genCapturesInDirection(std::unordered_map<Position, Move> &captures, Position end,
                                  Move::Direction direction) {
        Move::Type moveType = Move::Type::CAPTURE;
        std::pair<int, int> directionPair = direction;
        int rowDiff = directionPair.first, columnDiff = directionPair.second;

        Position initialPosition = position();
        int startRow = initialPosition.row(), row = startRow;
        int startColumn = initialPosition.column(), column = startColumn;
        int endRow = end.row();
        int endColumn = end.column();

        int rowMinBound = std::min(startRow, endRow);
        int rowMaxBound = std::max(startRow, endRow);
        int columnMinBound = std::min(startColumn, endColumn);
        int columnMaxBound = std::max(startColumn, endColumn);

        auto isFinalPositionInBounds = [&row, &rowMinBound, &rowMaxBound, &column, &columnMinBound,
                                        &columnMaxBound]() {
            return (row >= rowMinBound) && (row <= rowMaxBound) && (column >= columnMinBound) &&
                   (column <= columnMaxBound);
        };
        while (isFinalPositionInBounds()) {
            Position finalPosition(row, column);
            row += rowDiff, column += columnDiff;
            if (finalPosition == initialPosition) continue;

            Move move = Move::createMove(*this, initialPosition, finalPosition, moveType);
            captures[finalPosition] = move;
        }
        return captures;
    }

    std::unordered_map<Position, Move> &
    Piece::genMovesInDirection(std::unordered_map<Position, Move> &moves, Position end,
                               Move::Direction direction,
                               std::unordered_map<Position, Move> &captures) {
        std::pair<int, int> directionPair = direction;
        int rowDiff = directionPair.first, columnDiff = directionPair.second;
        std::unordered_map<Position, Piece *> &allies = this->friendlies();
        std::unordered_map<Position, Piece *> &opponents = this->opponents();

        Position initialPosition = position();
        int startRow = initialPosition.row(), row = startRow;
        int startColumn = initialPosition.column(), column = startColumn;
        int endRow = end.row();
        int endColumn = end.column();

        int rowMinBound = std::min(startRow, endRow);
        int rowMaxBound = std::max(startRow, endRow);
        int columnMinBound = std::min(startColumn, endColumn);
        int columnMaxBound = std::max(startColumn, endColumn);

        auto isFinalPositionInBounds = [&row, &rowMinBound, &rowMaxBound, &column, &columnMinBound,
                                        &columnMaxBound]() {
            return (row >= rowMinBound) && (row <= rowMaxBound) && (column >= columnMinBound) &&
                   (column <= columnMaxBound);
        };
        while (isFinalPositionInBounds()) {
            Position finalPosition(row, column);
            row += rowDiff, column += columnDiff;
            if (finalPosition == initialPosition) continue;

            if (allies.count(finalPosition)) break;

            if (opponents.count(finalPosition)) {
                if (captures.count(finalPosition)) {
                    Piece *opponent = &*opponents.at(finalPosition);
                    Move &capture = captures.at(finalPosition);
                    Move::addAction(capture, &*opponent, finalPosition);
                    moves[finalPosition] = capture;
                }
                break;
            }
            Move move =
                Move::createMove(*this, initialPosition, finalPosition, Move::Type::DISPLACEMENT);
            moves[finalPosition] = move;
        }
        return moves;
    }

    bool Piece::isInBounds(const Position &position, int rowMaxBound, int columnMaxBound,
                           int rowMinBound, int columnMinBound) {
        return position.row() >= rowMinBound && position.row() < rowMaxBound &&
               position.column() >= columnMinBound && position.column() < columnMaxBound;
    }

    int Piece::hash() const { return std::hash<std::string>()(*this); };

    bool Piece::operator==(const Piece &other) const { return this->hash() == other.hash(); };

    bool Piece::operator!=(const Piece &other) const { return !(*this == other); };

    std::ostream &operator<<(std::ostream &os, const Pieces::Piece *piece) {
        std::string pieceStr = *piece;
        os << pieceStr;
        return os;
    }

    Piece::operator std::string() const {
        std::string type = _type;
        std::string position = this->_position;
        std::string owner = this->isPlayerNullptr() ? "nullptr" : std::string(*this->_owner);
        return "Piece(" + type + ", " + position + ", " + owner + ")";
    }

} // namespace Pieces
