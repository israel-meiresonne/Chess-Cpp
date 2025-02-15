#include "position.hpp"

Position::Position()
    : _row(0)
    , _column(0) {};

Position::Position(int row, int column)
    : _row(row)
    , _column(column) {};

int Position::row() const { return _row; };

int Position::column() const { return _column; };

bool Position::operator==(const Position &other) const {
    return _row == other._row && _column == other._column;
}
