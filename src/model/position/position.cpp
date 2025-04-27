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

bool Position::operator!=(const Position &other) const { return !(*this == other); }

Position::operator std::string() const {
    return "Position(" + std::to_string(_row) + ", " + std::to_string(_column) + ")";
}

std::ostream &operator<<(std::ostream &os, const Position &position) {
    std::string positionStr = position;
    return os << positionStr;
}
