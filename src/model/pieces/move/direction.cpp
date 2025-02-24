#include "model/pieces/pieces.hpp"

namespace Pieces {
    const Move::Direction Move::Direction::UP = Direction(Direction::_Direction::UP);
    const Move::Direction Move::Direction::DOWN = Direction(Direction::_Direction::DOWN);
    const Move::Direction Move::Direction::LEFT = Direction(Direction::_Direction::LEFT);
    const Move::Direction Move::Direction::RIGHT = Direction(Direction::_Direction::RIGHT);
    const Move::Direction Move::Direction::UP_LEFT = Direction(Direction::_Direction::UP_LEFT);
    const Move::Direction Move::Direction::UP_RIGHT = Direction(Direction::_Direction::UP_RIGHT);
    const Move::Direction Move::Direction::DOWN_LEFT = Direction(Direction::_Direction::DOWN_LEFT);
    const Move::Direction Move::Direction::DOWN_RIGHT =
        Direction(Direction::_Direction::DOWN_RIGHT);

    Move::Direction::Direction()
        : _direction(_Direction::UNDEFINED){};
    Move::Direction::Direction(_Direction direction)
        : _direction(direction) {}

    Move::Direction::operator std::pair<int, int>() const {
        switch (_direction) {
        case _Direction::UP:
            return {1, 0};
        case _Direction::DOWN:
            return {-1, 0};
        case _Direction::LEFT:
            return {0, -1};
        case _Direction::RIGHT:
            return {0, 1};
        case _Direction::UP_LEFT:
            return {1, -1};
        case _Direction::UP_RIGHT:
            return {1, 1};
        case _Direction::DOWN_LEFT:
            return {-1, -1};
        case _Direction::DOWN_RIGHT:
            return {-1, 1};
        default:
            throw std::runtime_error("Undefined direction: '" +
                                     std::to_string(static_cast<int>(_direction)) + "'");
        }
    }

} // namespace Pieces
