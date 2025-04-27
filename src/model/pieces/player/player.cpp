#include "model/pieces/pieces.hpp"

namespace Pieces {
    Player::Player() {};

    Player::Player(const std::string &name)
        : _name(name) {};

    std::string Player::name() const { return _name; };

    int Player::hash() const { return std::hash<std::string>()(_name); }

    bool Player::operator==(const Player &other) const { return this->hash() == other.hash(); }
    
    bool Player::operator!=(const Player &other) const { return !(*this == other); }

    Player::operator std::string() const { return "Player(" + _name + ")"; }

    std::ostream &operator<<(std::ostream &os, const Player &player) {
        std::string playerStr = player;
        os << playerStr;
        return os;
    }
} // namespace Pieces
