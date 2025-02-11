#include "model/player/player.hpp"

#include <iostream>

int main() {
  Player player;
  std::cout << "Hello " << player.name() << std::endl;

  Player anotherPlayer("Alice");
  std::cout << "Hello " << anotherPlayer.name() << std::endl;

  return 0;
}