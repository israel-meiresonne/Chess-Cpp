#include "view/cli/cli.hpp"

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace View {
    CLI::CLI() {}

    std::string CLI::input() {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    std::string CLI::inputChecker(const std::string &regex, const std::string &errorMessage) {
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

    std::string CLI::inputSecret() {
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

    std::string CLI::menu(const std::vector<std::string> &choices) {
        int i = 1;
        std::string choiceStr;
        for (const auto choice : choices) {
            choiceStr = std::to_string(i) + ". " + choice;
            this->output(choiceStr);
            i++;
        }

        int choice = 0;
        auto errorMessages = [this]() {
            this->output("Invalid choice!", Style::ERROR);
            this->output("Try again:");
        };
        while (true) {
            try {
                choice = std::stoi(this->input());
                if (choice >= 1 && static_cast<size_t>(choice) <= choices.size()) break;

                errorMessages();
            } catch (const std::exception &e) {
                errorMessages();
            }
        }

        return choices[choice - 1];
    }

    void CLI::output(const std::string &message, Style style) {
        std::cout << std::string(style) + message + std::string(Style::NORMAL) << std::endl;
    }

    void CLI::outputGrid(const std::vector<std::vector<std::string>> &grid) {
        for (const auto rows : grid) {
            for (const auto column : rows) {
                std::cout << column;
            }
            std::cout << std::endl;
        }
    }
} // namespace View
