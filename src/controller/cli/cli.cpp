#include "controller/cli/cli.hpp"

namespace Controller {
    CLI::CLI() {};

    void CLI::start() {
        std::string choice;
        while (true) {
            choice = this->mainMenu();
            if (choice == "Quit") break;

            if (choice == "Start new game") this->startNewGame();
        }
        this->_view.success("Chess game successfully ends!");
    };

    std::string CLI::mainMenu() {
        auto &view = this->_view;
        std::vector<std::string> menu = {"Start new game", "Quit"};
        view.output("Main Menu:");
        return view.menu(menu);
    }

    void CLI::startNewGame() {
        auto &view = this->_view;
        std::string errorMessage = "Allowed characters: a-zA-Z0-9_";

        view.output("Player 1, enter your name:");
        auto namePlayer1 = view.inputChecker("^\\w+$", errorMessage);

        view.output("Player 2, enter your name:");
        auto namePlayer2 = view.inputChecker("^\\w+$", errorMessage);

        this->playGame(namePlayer1, namePlayer2);
    }

    void CLI::playGame(std::string &namePlayer1, std::string &namePlayer2) {
        auto &view = this->_view;
        this->_model = Game::Game();
        auto &model = this->_model;
        model.start(namePlayer1, namePlayer2);

        std::string mainChoice;
        std::vector<std::string> mainMenu = {"Select a piece", "Quit"};
        std::string moveError;

        std::string playerName;
        Position from;
        Position to;
        auto gameEnded = [&model]() { return model.status() != Game::Status::IN_PROGRESS; };
        while (!gameEnded()) {
            this->showBoard();
            playerName = model.currentPlayer().name();

            view.output(playerName + ", select an option:");
            mainChoice = view.menu(mainMenu);
            if (mainChoice == "Quit") break;

            from = this->inputPosition(playerName + ", select a piece by its row and column:");
            to = this->inputPosition(playerName + ", select a destination by its row and column:");

            moveError = this->movePiece(from, to);
            if (static_cast<int>(moveError.size()) == 0) {
                view.success(playerName + ", pieces successfully moves to " +
                             std::to_string(to.row()) + "," + std::to_string(to.column()));
                continue;
            }
            view.error("Couldn't move the piece because of: " + moveError);
            view.error("Try again!");
        }
        if (mainChoice == mainMenu[1]) return view.warn(playerName + " quitted the game!");

        this->evaluateEdnGame();
    }

    void CLI::evaluateEdnGame() const {
        auto &view = this->_view;
        auto &model = this->_model;
        auto status = model.status();
        if (status == Game::Status::ENDED_STALEMATE) {
            return view.warn("Game ended on STALEMATE, no winner!");
        }
        if (status == Game::Status::ENDED_CHECKMATE) {
            auto currentPlayer = model.currentPlayer();
            auto winner = (currentPlayer == model.player1()) ? model.player2() : currentPlayer;
            return view.success(winner.name() + "has won by CHECKMATE!");
        }
        throw std::runtime_error("Cannot evaluate this game status. status: '" +
                                 std::string(status) + "'");
    }
    void CLI::showBoard() const {
        const auto &serializedBoard = this->_model.board();
        const auto boardNRow = static_cast<int>(serializedBoard.size());
        const auto boardNCol = static_cast<int>(serializedBoard[0].size());
        int boardRow;
        int boardCol;

        int gridNRow = boardNRow * 2 + 1;
        int gridNCol = boardNCol * 2 + 1;

        std::vector<std::string> gridColumns(gridNCol, " ");
        std::vector<std::vector<std::string>> grid(gridNRow, gridColumns);

        std::vector<std::string> logs;
        auto buildLogs = [&logs]() {
            std::string built;
            for (auto &log : logs) {
                built += "\n" + log;
            }
            return built;
        };
        for (int i = 0; i < gridNRow; ++i) {
            for (int j = 0; j < gridNCol; ++j) {
                if (this->isColLabel(gridNRow, i, j)) {
                    grid[i][j] = '0' + (j / 2 - 1);
                    continue;
                }
                if (this->isRowLabel(gridNRow, i, j)) {
                    grid[i][j] = '0' + (boardNRow - 1 - i / 2);
                    continue;
                }
                if (this->isHorizontalBorder(i, j)) {
                    grid[i][j] = '-';
                    continue;
                }
                if (this->isVerticalBorder(i, j)) {
                    grid[i][j] = '|';
                    continue;
                }
                if (this->isCrossPoint(i, j)) {
                    grid[i][j] = '+';
                    continue;
                }
                if (this->isPieceCell(i, j, gridNRow, gridNCol)) {
                    boardRow = boardNRow - i / 2 - 1;
                    boardCol = j / 2 - 1;
                    auto *piece = serializedBoard[boardRow][boardCol];
                    grid[i][j] = (piece != nullptr) ? piece->icon() : " ";
                    continue;
                }
                logs.push_back("boardNRow: '" + std::to_string(boardNRow) + "'");
                logs.push_back("boardNCol: '" + std::to_string(boardNCol) + "'");
                logs.push_back("gridNRow: '" + std::to_string(gridNRow) + "'");
                logs.push_back("gridNCol: '" + std::to_string(gridNCol) + "'");
                logs.push_back("i: '" + std::to_string(i) + "'");
                logs.push_back("j: '" + std::to_string(j) + "'");
                this->_view.outputGrid(grid);
                throw std::runtime_error("Unsupported grid case." + buildLogs());
            }
        }
        this->_view.outputGrid(grid);
    }

    Position CLI::inputPosition(const std::string &message) const {
        auto &view = this->_view;
        view.output(message);
        auto input = view.inputChecker(
            "^[0-7],[0-7]$",
            "Position must be 2 digits between [0-7] separated by a commas like '7,0'.");
        int row = std::stoi(input.substr(0, 1)), column = std::stoi(input.substr(2));
        return Position(row, column);
    }

    std::string CLI::movePiece(const Position &from, const Position &to) {
        std::string errorMessage;
        try {
            this->_model.move(from, to);
        } catch (const std::exception &e) {
            errorMessage = e.what();
        }
        return errorMessage;
    }

    bool CLI::isEven(int x) const { return x % 2 == 0; }
    bool CLI::isOdd(int x) const { return x % 2 != 0; }
    bool CLI::isPieceCell(int i, int j, int gridNRow, int gridNCol) const {
        return isEven(i) && isEven(j) && i < gridNRow - 2;
    }
    bool CLI::isColLabel(int gridNRow, int i, int j) const {
        return i == (gridNRow - 1) && isEven(j);
    }
    bool CLI::isRowLabel(int gridNRow, int i, int j) const {
        return j == 0 && isEven(i) && i < gridNRow - 2;
    }
    bool CLI::isHorizontalBorder(int i, int j) const { return isOdd(i) && (isEven(j) || j == 0); }
    bool CLI::isVerticalBorder(int i, int j) const { return (i == 0 || isEven(i)) && isOdd(j); }
    bool CLI::isCrossPoint(int i, int j) const { return isOdd(i) && isOdd(j); }

} // namespace Controller
