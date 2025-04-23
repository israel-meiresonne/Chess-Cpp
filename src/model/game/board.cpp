#include "game.hpp"

namespace Game {
    Board::Board()
        : _boundaries({0, 0})
        , _nMoves(0)
        , _status(Status::NOT_STARTED)
        , _moves()
        , _pieces() {}

    Board::Board(int nRow, int nColumn)
        : _boundaries({nColumn, nRow})
        , _nMoves(0)
        , _status(Status::NOT_STARTED)
        , _moves()
        , _pieces() {}

    Board::~Board() {
        for (auto *piece : _pieces) {
            delete piece;
        }
        _pieces.clear();
    }

    std::pair<int, int> Board::boundaries() const { return this->_boundaries; }

    Status Board::status() const { return this->_status; }

    const std::vector<Pieces::Piece *> &Board::pieces() const { return this->_pieces; }

    void Board::initialize(Pieces::Player &first, Pieces::Player &second) {
        this->_pieces = this->initializePieces(first, true);
        auto secondPieces = this->initializePieces(second, false);
        this->_pieces.insert(this->_pieces.end(), secondPieces.begin(), secondPieces.end());
        this->updateStatus(first);
    }

    void Board::move(Pieces::Piece *piece, Position to) {
        if (this->_status != Status::IN_PROGRESS) {
            throw std::runtime_error("Board's status must be '" + std::string(Status::IN_PROGRESS) +
                                     "', instead status='" + std::string(this->_status) + "'");
        }

        this->pieceExists(piece);

        auto moves = this->moves(piece, to);
        auto actions = this->actions(piece, to, moves);

        Pieces::Player *owner = piece->owner();
        this->playMove(*owner, actions);

        Pieces::Move &move = moves.at(to);
        this->_moves.do_(move);

        this->updateStatus(*owner);
    }

    void Board::updateStatus(Pieces::Player &player) {
        if ((this->_status == Status::NOT_STARTED) && !this->_moves.canUndo()) {
            this->_status = Status::IN_PROGRESS;
            return;
        }

        bool kingHasMoves = this->opponentKingHasMoves(player);
        bool kingIsThreatened = this->isOpponentKingThreatened(player);
        if (!kingHasMoves && kingIsThreatened) {
            this->_status = Status::ENDED_CHECKMATE;
            return;
        }

        bool kingIsLastPiece = this->opponentKingIsLastPiece(player);
        if (!kingHasMoves && !kingIsThreatened && kingIsLastPiece) {
            this->_status = Status::ENDED_STALEMATE;
            return;
        }

        return;
    }

    void Board::unMove() {
        Pieces::Move move = this->_moves.undo();
        this->_nMoves--;

        auto owner = move.actions()[0].piece()->owner();
        this->updateStatus(*owner);
    }

    void Board::reMove() {
        Pieces::Move move = this->_moves.redo();
        this->_nMoves++;

        auto owner = move.actions()[0].piece()->owner();
        this->updateStatus(*owner);
    }

    std::unordered_map<Position, Pieces::Piece *> Board::promotions(Pieces::Piece *piece) {
        std::unordered_map<Position, Pieces::Piece *> promotions;
        return promotions;
    }

    void Board::promote(Pieces::Piece *piece, Pieces::Types promotion) {}

    bool Board::opponentKingIsLastPiece(Pieces::Player &player) {
        auto opponents = this->playerPieces(player, false);
        return (opponents.size() == 1) && findKing(opponents);
    }

    bool Board::opponentKingHasMoves(Pieces::Player &player) {
        auto opponents = this->playerPieces(player, false);
        auto king = findKing(opponents);

        auto friendlies = this->playerPieces(player, true);
        auto moves =
            king->moves(opponents, this->_boundaries.first, this->_boundaries.second, friendlies);
        return !moves.empty();
    }

    bool Board::isOpponentKingThreatened(Pieces::Player &player) {
        auto opponents = this->playerPieces(player, false);
        auto king = findKing(opponents);
        auto kingPosition = king->position();

        auto friendlies = this->playerPieces(player, true);
        auto nRow = this->_boundaries.first;
        auto nColumn = this->_boundaries.second;
        std::unordered_map<Position, Pieces::Move> moves;
        for (auto &[_, friendly] : friendlies) {
            moves = friendly->moves(friendlies, nRow, nColumn, opponents);
            if (!moves.count(kingPosition)) continue;

            return true;
        }
        return false;
    }

    static Pieces::Piece *findKing(std::unordered_map<Position, Pieces::Piece *> pieces) {
        std::vector<Pieces::Piece *> vPieces;
        for (auto &[_, piece] : pieces) {
            vPieces.push_back(piece);
        }
        return findPieces(vPieces, Pieces::Types::KING, true)[0];
    }

    static std::vector<Pieces::Piece *> findPieces(std::vector<Pieces::Piece *> pieces,
                                                   Pieces::Types type, bool throwNotFound = false) {
        std::vector<Pieces::Piece *> founds;
        for (auto *piece : pieces) {
            if (piece->type() != type) continue;

            founds.push_back(piece);
        }
        if (founds.size() == 0) {
            throw std::runtime_error("Piece of type '" + std::string(type) + "' not found");
        }

        return founds;
    }

    std::vector<Pieces::Piece *> Board::initializePieces(Pieces::Player &player,
                                                         bool isFirstPlayer) {
        std::vector<Pieces::Piece *> pieces;
        int initialRow = isFirstPlayer ? this->_boundaries.first - 1 : 0;
        int pawnRow = isFirstPlayer ? initialRow - 1 : 1;

        pieces.push_back(new Pieces::Rook(Position(initialRow, 0), &player));
        pieces.push_back(new Pieces::Knight(Position(initialRow, 1), &player));
        pieces.push_back(new Pieces::Bishop(Position(initialRow, 2), &player));
        pieces.push_back(new Pieces::Queen(Position(initialRow, 3), &player));
        pieces.push_back(new Pieces::King(Position(initialRow, 4), &player));
        pieces.push_back(new Pieces::Bishop(Position(initialRow, 5), &player));
        pieces.push_back(new Pieces::Knight(Position(initialRow, 6), &player));
        pieces.push_back(new Pieces::Rook(Position(initialRow, 7), &player));

        for (int col = 0; col < 8; ++col) {
            pieces.push_back(new Pieces::Pawn(Position(pawnRow, col), &player));
        }

        return pieces;
    }

    bool Board::pieceExists(Pieces::Piece *piece) {
        auto &pieces = this->_pieces;
        if (std::find(pieces.begin(), pieces.end(), piece) != pieces.end()) return true;

        std::string pieceStr = *piece;
        throw std::runtime_error("This Piece does not exist: piece='" + pieceStr + "'");
    }

    std::unordered_map<Position, Pieces::Piece *> Board::playerPieces(Pieces::Player &player,
                                                                      bool isFriendly) {
        std::unordered_map<Position, Pieces::Piece *> mPieces;
        for (auto *piece : this->_pieces) {
            if ((*piece->owner() == player) != isFriendly) continue;

            mPieces[piece->position()] = piece;
        }
        return mPieces;
    }

    std::unordered_map<Position, Pieces::Move> Board::moves(Pieces::Piece *piece, Position &to) {
        Pieces::Player *owner = piece->owner();
        auto opponents = this->playerPieces(*owner, false);
        auto friendlies = this->playerPieces(*owner, true);
        auto moves = piece->moves(friendlies, _boundaries.first, _boundaries.second, opponents);

        if (!moves.count(to)) {
            std::string toStr = to, pieceStr = *piece;
            throw std::runtime_error("No move to the given destination is available: to='" + toStr +
                                     "', piece='" + pieceStr + "'");
        }

        return moves;
    }

    std::vector<Pieces::Action> Board::actions(Pieces::Piece *piece, Position &to,
                                               std::unordered_map<Position, Pieces::Move> &moves) {
        Pieces::Move &move = moves.at(to);
        std::vector<Pieces::Action> actions = move.actions();

        Pieces::Piece *actionPiece = actions[0].piece();
        if (*actionPiece != *piece) {
            std::string actionPieceStr = *actionPiece, pieceStr = *piece;
            throw std::runtime_error(
                "The moving piece and the action piece must be the same: piece='" + pieceStr +
                "', actionPiece='" + actionPieceStr + "'");
        }

        return actions;
    }

    void Board::playMove(Pieces::Player &player, std::vector<Pieces::Action> &actions) {
        Pieces::Piece *actionPiece = nullptr;
        for (auto &action : actions) {
            actionPiece = action.piece();
            actionPiece->move(action.final());
        }
    }

    void Board::unPlayMove() {};
} // namespace Game
