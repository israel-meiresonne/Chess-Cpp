#include "model/pieces/pieces.hpp"

namespace Pieces {
    Move::Move()
        : _type(Move::Type::DISPLACEMENT)
        , _author(Player()) {}

    Move::Move(const Player &author, const Type type)
        : _author(author)
        , _type(type) {}

    Move::Type Move::type() const { return _type; }
    std::vector<Action> Move::actions() const { return _actions; }
    Player Move::author() const { return _author; }

    void Move::add(const Action &action) { _actions.push_back(action); }

    bool Move::operator==(const Move &other) const {
        return _type == other._type && _author == other._author && _actions == other._actions;
    }

    int Move::hash() const {
        return (static_cast<int>(_type) << 1) ^ _author.hash() ^
               (Utils::Templates::hash_vector<Action>(_actions) << 2);
    }
} // namespace Pieces
