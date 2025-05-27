#ifndef VIEW_CLI_HPP
#define VIEW_CLI_HPP

#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

namespace View {
    class CLI {
      public:
        class Style {
            enum class _Style { NORMAL, WARNING, ERROR, SUCCESS };

            _Style _style;

            explicit Style(_Style style);

          public:
            static const Style NORMAL;
            static const Style WARNING;
            static const Style ERROR;
            static const Style SUCCESS;

            _Style style() const;

            bool operator==(const Style &other) const;

            bool operator!=(const Style &other) const;

            operator std::string() const;
        };

        CLI();

        std::string input() const;
        std::string inputChecker(const std::string &regex, const std::string &errorMessage) const;
        std::string inputSecret() const;
        std::string menu(const std::vector<std::string> &choices) const;
        void output(const std::string &message, Style style = Style::NORMAL) const;
        void success(const std::string &message) const;
        void warn(const std::string &message) const;
        void error(const std::string &message) const;
        void outputGrid(const std::vector<std::vector<std::string>> &grid) const;
    };

} // namespace View

#endif // VIEW_CLI_HPP
