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

    std::ostream &operator<<(std::ostream &os, const Pieces::Action &action) {
        if (action.isPieceNullptr()) {
            os << "Action(nullptr, From: " << action.initial() << " To: " << action.final() << ")";
        } else {
            os << "Action(" << action.piece() << ", From: " << action.initial()
               << " To: " << action.final() << ")";
        }
        return os;
    }

} // namespace Pieces
