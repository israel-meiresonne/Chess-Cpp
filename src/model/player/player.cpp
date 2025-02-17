#include "player.hpp"

Player::Player() {};

Player::Player(const std::string &name)
    : _name(name) {};

std::string Player::name() const { return _name; };

bool Player::operator==(const Player &other) const { return _name == other._name; }

int Player::hash() const { return std::hash<std::string>()(_name); }
