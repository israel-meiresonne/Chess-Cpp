#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iostream>

namespace Pieces {
    class Player {
      public:
        Player();
        Player(const std::string &name);

        std::string name() const;

        int hash() const;

        bool operator==(const Player &other) const;

        operator std::string() const;

        friend std::ostream &operator<<(std::ostream &os, const Player &player);

      private:
        std::string _name;
    };
} // namespace Pieces

#endif // PLAYER_HPP
