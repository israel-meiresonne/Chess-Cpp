#include "position.hpp"

Position::Position() : _row(0), _column(0) {};

Position::Position(int row, int column) : _row(row), _column(column) {};

int Position::row() const { return this->_row; };

int Position::column() const { return this->_column; };
