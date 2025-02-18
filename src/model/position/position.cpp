#include "position.hpp"

Position::Position()
    : _row(-1)
    , _column(-1) {};

Position::Position(int row, int column)
    : _row(row)
    , _column(column) {};

int Position::row() const { return _row; };

int Position::column() const { return _column; };

int Position::hash() const { return _row ^ (_column << 1); }

bool Position::operator==(const Position &other) const {
    return _row == other._row && _column == other._column;
}

std::ostream &operator<<(std::ostream &os, const Position &pos) {
    os << "Position(" << pos.row() << ", " << pos.column() << ")";
    return os;
}
