#include "model/pieces/pieces.hpp"

namespace Pieces {
    Move::Move()
        : _author(Player()) {}

    Move::Move(const Player &author)
        : _author(author) {}

    std::vector<Action> Move::actions() const { return _actions; }

    Player Move::author() const { return _author; }

    void Move::add(const Action &action) { _actions.push_back(action); }
} // namespace Pieces
