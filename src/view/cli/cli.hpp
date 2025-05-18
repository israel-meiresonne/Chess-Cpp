#ifndef CLI_HPP
#define CLI_HPP

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

        std::string input();
        std::string inputChecker(const std::string &regex, const std::string &errorMessage);
        std::string inputSecret();
        std::string menu(const std::vector<std::string> &choices);
        void output(const std::string &message, Style style = Style::NORMAL);
        void outputGrid(const std::vector<std::vector<std::string>> &grid);
    };

} // namespace View

#endif // CLI_HPP
