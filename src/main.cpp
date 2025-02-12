#include "model/player/player.hpp"
#include "model/position/position.hpp"

#include <iostream>

int main() {
  Position position;
  std::cout << "Position(row, column): " << position.row() << ","
            << position.column() << std::endl;

  Position position2(1, 2);
  std::cout << "Position(row, column): " << position2.row() << ","
            << position2.column() << std::endl;
  return 0;
}