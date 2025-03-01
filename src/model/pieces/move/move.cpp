#include "model/pieces/pieces.hpp"

namespace Pieces {
    Move::Move()
        : _type(Type::DISPLACEMENT) {}
    Move::Move(const Type type)
        : _type(type) {}

    Move::Type Move::type() const { return _type; }
    std::vector<Action> Move::actions() const { return _actions; }

    void Move::add(const Action &action) { _actions.push_back(action); }

    int Move::hash() const {
        return (static_cast<int>(_type) << 1) ^
               (Utils::Templates::hash_vector<Action>(_actions) << 2);
    }

    bool Move::operator==(const Move &other) const {
        return _type == other._type && _actions == other._actions;
    }

    std::ostream &operator<<(std::ostream &os, const Pieces::Move::Type &moveType) {
        switch (moveType) {
        case Pieces::Move::Type::DISPLACEMENT:
            os << "DISPLACEMENT";
            break;
        case Pieces::Move::Type::CAPTURE:
            os << "CAPTURE";
            break;
        case Pieces::Move::Type::SWAP:
            os << "SWAP";
            break;
        default:
            os << "Unknown";
            break;
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Pieces::Move &move) {
        os << "Move(Type: " << move.type() << ", Actions: [";
        auto actions = move.actions();
        for (const auto &action : actions) {
            os << action << (action == actions.back() ? "" : ", ");
        }
        os << "])";
        return os;
    }

    Move Move::createMove(Pieces::Piece &piece, Position initial, Position final,
                          Pieces::Move::Type moveType) {
        Pieces::Action action(&piece, initial, final);
        Pieces::Move move(moveType);
        move.add(action);
        return move;
    }

    Move &Move::addAction(Move &move, const Piece *piece, Position &initial, Position final) {
        Action captureOpponent = Action(&*piece, initial, final);
        move.add(captureOpponent);
        return move;
    }

} // namespace Pieces
