#include "view/cli/cli.hpp"

namespace View {
    const CLI::Style CLI::Style::NORMAL = CLI::Style(CLI::Style::_Style::NORMAL);
    const CLI::Style CLI::Style::WARNING = CLI::Style(CLI::Style::_Style::WARNING);
    const CLI::Style CLI::Style::ERROR = CLI::Style(CLI::Style::_Style::ERROR);
    const CLI::Style CLI::Style::SUCCESS = CLI::Style(CLI::Style::_Style::SUCCESS);

    CLI::Style::Style(_Style style)
        : _style(style) {}

    CLI::Style::_Style CLI::Style::style() const { return this->_style; };

    bool CLI::Style::operator==(const Style &other) const { return this->_style == other._style; }

    bool CLI::Style::operator!=(const Style &other) const { return !(*this == other); }

    CLI::Style::operator std::string() const {
        switch (this->_style) {
        case _Style::NORMAL:
            return "\033[0m";
        case _Style::WARNING:
            return "\033[33m";
        case _Style::ERROR:
            return "\033[31m";
        case _Style::SUCCESS:
            return "\033[32m";
        default:
            throw std::runtime_error("Undefined CLI::Style: '" +
                                     std::to_string(static_cast<int>(this->_style)) + "'");
        }
    }
} // namespace View
