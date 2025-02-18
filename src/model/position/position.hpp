#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>

#include "model/utils/templates.hpp"

class Position {
  private:
    int _row;
    int _column;

  public:
    Position();
    Position(int row, int column);

    int row() const;
    int column() const;

    int hash() const;

    bool operator==(const Position &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Position &pos);
};

#endif // POSITION_HPP
