#include "model/pieces/pieces.hpp"

namespace Pieces {

    Action::Action()
        : _piece(nullptr)
        , _initial(Position())
        , _final(Position()) {}

    Action::Action(Pieces::Piece *piece, const Position &initial, const Position &final)
        : _piece(piece)
        , _initial(initial)
        , _final(final) {}

    Pieces::Piece *Action::piece() const { return _piece; }

    Position Action::initial() const { return _initial; }

    Position Action::final() const { return _final; }

    bool Action::operator==(const Action &other) const {
        return _piece == other._piece && _initial == other._initial && _final == other._final;
    }
} // namespace Pieces
