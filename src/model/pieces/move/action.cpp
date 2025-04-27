#include "model/pieces/pieces.hpp"

namespace Pieces {

    Action::Action()
        : _piece(nullptr)
        , _initial(Position())
        , _final(Position()) {}

    Action::Action(Pieces::Piece *piece, const Position initial, const Position final)
        : _piece(piece)
        , _initial(initial)
        , _final(final) {}

    bool Action::isPieceNullptr() const { return _piece == nullptr; }

    Pieces::Piece *Action::piece() const {
        if (isPieceNullptr()) throw std::runtime_error("Piece is nullptr");

        return &*_piece;
    }

    Position Action::initial() const { return _initial; }

    Position Action::final() const { return _final; }

    int Action::hash() const {
        int piece_hash = _piece == nullptr ? 0 : _piece->hash();

        return piece_hash ^ (_initial.hash() << 1) ^ (_final.hash() << 2);
    }

    bool Action::operator==(const Action &other) const {
        return _piece == other._piece && _initial == other._initial && _final == other._final;
    }

    Action::operator std::string() const {
        std::string str, from = this->initial(), to = this->final();

        if (this->isPieceNullptr()) return "Action(nullptr, From: " + from + " To: " + to + ")";

        return "Action(" + std::string(*this->piece()) + ", From: " + from + " To: " + to + ")";
    }

    std::ostream &operator<<(std::ostream &os, const Pieces::Action &action) {
        return os << std::string(action);
    }

} // namespace Pieces
