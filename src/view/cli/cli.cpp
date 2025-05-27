#include "view/cli/cli.hpp"

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace View {
    CLI::CLI() {}

    std::string CLI::input() const {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    std::string CLI::inputChecker(const std::string &regex, const std::string &errorMessage) const {
        std::regex regexPattern(regex);
        std::string inputValue;
        while (true) {
            std::getline(std::cin, inputValue);
            if (std::regex_match(inputValue, regexPattern)) break;

            this->output(errorMessage, Style::ERROR);
            this->output("Try again:");
        }
        return inputValue;
    }

    std::string CLI::inputSecret() const {
        std::string secret;

#ifdef _WIN32
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b' && !secret.empty()) {
                secret.pop_back();
                std::cout << "\b \b";
            } else if (isprint(ch)) {
                secret += ch;
                std::cout << '*';
            }
        }
#else
        termios oldt{}, newt{};
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        std::getline(std::cin, secret);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

        std::cout << std::endl;
        return secret;
    }

    std::string CLI::menu(const std::vector<std::string> &choices) const {
        int i = 1;
        std::string choiceStr;
        for (const auto choice : choices) {
            choiceStr = std::to_string(i) + ". " + choice;
            this->output(choiceStr);
            i++;
        }

        int choice = 0;
        std::string errorMessage("Invalid choice!");
        auto errorMessages = [this, &errorMessage]() {
            this->output(errorMessage, Style::ERROR);
            this->output("Try again:");
        };
        std::string regex("^\\d+$");
        while (true) {
            try {
                choice = std::stoi(this->inputChecker(regex, errorMessage));
                if (choice >= 1 && choice <= static_cast<int>(choices.size())) break;
            } catch (const std::exception &e) {
                errorMessages();
            }
        }

        return choices[choice - 1];
    }

    void CLI::output(const std::string &message, Style style) const {
        std::cout << std::string(style) + message + std::string(Style::NORMAL) << std::endl;
    }

    void CLI::success(const std::string &message) const { this->output(message, Style::SUCCESS); }
    void CLI::warn(const std::string &message) const { this->output(message, Style::WARNING); }
    void CLI::error(const std::string &message) const { this->output(message, Style::ERROR); }

    void CLI::outputGrid(const std::vector<std::vector<std::string>> &grid) const {
        for (const auto rows : grid) {
            for (const auto column : rows) {
                std::cout << column;
            }
            std::cout << std::endl;
        }
    }
} // namespace View
