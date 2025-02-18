#include "model/pieces/pieces.hpp"

namespace Pieces {
    Move::Move()
        : _author(Player())
        , _type(Move::Type::DISPLACEMENT) {}

    Move::Move(const Player &author, const Type &type)
        : _author(author)
        , _type(type) {}

    Move::Move(const Type &type, const Player &author)
        : _author(author)
        , _type(type) {}

    Move::Type Move::type() const { return _type; }
    std::vector<Action> Move::actions() const { return _actions; }
    Player Move::author() const { return _author; }

    void Move::add(const Action &action) { _actions.push_back(action); }

    int Move::hash() const {
        return (static_cast<int>(_type) << 1) ^ _author.hash() ^
               (Utils::Templates::hash_vector<Action>(_actions) << 2);
    }

    bool Move::operator==(const Move &other) const {
        return _type == other._type && _author == other._author && _actions == other._actions;
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
        os << "Move(Type: " << move.type() << ", Author: " << move.author() << ", Actions: [";
        auto actions = move.actions();
        for (const auto &action : actions) {
            os << action << " ";
        }
        os << "])";
        return os;
    }

    Move Move::createMove(Pieces::Piece *piece, Position initial, Position final,
                          Pieces::Move::Type moveType) {
        Pieces::Action action(piece, initial, final);
        Pieces::Move move(moveType);
        move.add(action);
        return move;
    }

} // namespace Pieces
