#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>

#include "model/pieces/pieces.hpp"
#include "model/position/position.hpp"

class Player {
  public:
    Player();
    Player(const std::string &name);

    std::string name() const;
    const std::unordered_map<Position, Pieces::Piece *> &pieces() const;

    int hash() const;

    bool operator==(const Player &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Player &player);

  private:
    std::string _name;
    std::unordered_map<Position, Pieces::Piece *> _pieces;
};

#endif // PLAYER_HPP
