#ifndef POSITION_HPP
#define POSITION_HPP

class Position {
  private:
    int _row;
    int _column;

  public:
    Position();
    Position(int row, int column);

    int row() const;
    int column() const;

    bool operator==(const Position &other) const;
};

#endif // POSITION_HPP
