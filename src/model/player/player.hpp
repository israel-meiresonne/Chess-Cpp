#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>

class Player {
  private:
    std::string _name;

  public:
    Player();
    Player(const std::string &name);

    std::string name() const;

    int hash() const;

    bool operator==(const Player &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Player &player);
};

#endif // PLAYER_HPP
