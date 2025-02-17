#include "model/pieces/pieces.hpp"

namespace Pieces {
    Move::Move()
        : _author(Player()) {}

    Move::Move(const Player &author)
        : _author(author) {}

    std::vector<Action> Move::actions() const { return _actions; }

    Player Move::author() const { return _author; }

    void Move::add(const Action &action) { _actions.push_back(action); }

    bool Move::operator==(const Move &other) const {
        return _author == other._author && _actions == other._actions;
    }

    int Move::hash() const {
        return _author.hash() ^ (Utils::Templates::hash_vector<Action>(_actions) << 1);
    }
} // namespace Pieces
