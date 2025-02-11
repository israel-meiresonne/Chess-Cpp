#include "player.hpp"

Player::Player() {};

Player::Player(const std::string &name) : _name(name) {};

std::string Player::name() const { return _name; };
