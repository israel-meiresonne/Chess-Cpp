#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
  private:
    std::string _name;

  public:
    Player();
    Player(const std::string &name);

    std::string name() const;

    bool operator==(const Player &other) const;
};

#endif // PLAYER_HPP
