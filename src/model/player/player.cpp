#include "player.hpp"

Player::Player() {};

Player::Player(const std::string &name)
    : _name(name) {};

std::string Player::name() const { return _name; };
const std::unordered_map<Position, Pieces::Piece *> &Player::pieces() const { return _pieces; };

int Player::hash() const { return std::hash<std::string>()(_name); }

bool Player::operator==(const Player &other) const { return _name == other._name; }

std::ostream &operator<<(std::ostream &os, const Player &player) {
    os << "Player(" << player.name() << ")";
    return os;
}
