#include "game.hpp"

namespace Game {
    const Status Status::NOT_STARTED = Status(Status::_Status::NOT_STARTED);
    const Status Status::IN_PROGRESS = Status(Status::_Status::IN_PROGRESS);
    const Status Status::ENDED_CHECKMATE = Status(Status::_Status::ENDED_CHECKMATE);
    const Status Status::ENDED_STALEMATE = Status(Status::_Status::ENDED_STALEMATE);
    const Status Status::UNDEFINED = Status(Status::_Status::UNDEFINED);

    Status::Status()
        : _status(_Status::UNDEFINED) {}
    Status::Status(_Status status)
        : _status(status) {}

    Status::operator std::string() const {
        switch (this->_status) {
        case _Status::NOT_STARTED:
            return "NOT_STARTED";
        case _Status::IN_PROGRESS:
            return "IN_PROGRESS";
        case _Status::ENDED_CHECKMATE:
            return "ENDED_CHECKMATE";
        case _Status::ENDED_STALEMATE:
            return "ENDED_STALEMATE";
        case _Status::UNDEFINED:
            return "UNDEFINED";
        default:
            throw std::runtime_error("Undefined Game::Status: '" +
                                     std::to_string(static_cast<int>(this->_status)) + "'");
        }
    }

    int Status::hash() const { return static_cast<int>(this->_status) << 1; }

    bool Status::operator==(const Status &other) const { return this->hash() == other.hash(); }

    bool Status::operator!=(const Status &other) const { return !(*this == other); }

} // namespace Game
