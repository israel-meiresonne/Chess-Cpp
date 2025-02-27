#include "model/pieces/pieces.hpp"

namespace Pieces {
    const Types Types::KING = Types(Types::_Types::KING);
    const Types Types::QUEEN = Types(Types::_Types::QUEEN);
    const Types Types::ROOK = Types(Types::_Types::ROOK);
    const Types Types::BISHOP = Types(Types::_Types::BISHOP);
    const Types Types::KNIGHT = Types(Types::_Types::KNIGHT);
    const Types Types::PAWN = Types(Types::_Types::PAWN);
    const Types Types::UNDEFINED = Types(Types::_Types::UNDEFINED);

    Types::Types(_Types type)
        : _type(type) {};

    Types::operator std::string() const {
        switch (_type) {
        case Types::_Types::KING:
            return "KING";
        case Types::_Types::QUEEN:
            return "QUEEN";
        case Types::_Types::ROOK:
            return "ROOK";
        case Types::_Types::BISHOP:
            return "BISHOP";
        case Types::_Types::KNIGHT:
            return "KNIGHT";
        case Types::_Types::PAWN:
            return "PAWN";
        case Types::_Types::UNDEFINED:
            return "UNDEFINED";
        default:
            throw std::runtime_error("Undefined Pieces::Types: '" +
                                     std::to_string(static_cast<int>(_type)) + "'");
        }
    }

    int Types::hash() const { return static_cast<int>(_type) << 1; }

    bool Types::operator==(const Types &other) const { return _type == other._type; };

} // namespace Pieces
