#ifndef CONTROLLER_CLI_HPP
#define CONTROLLER_CLI_HPP

#include <string>

#include "model/game/game.hpp"
#include "view/view.hpp"

namespace Controller {
    class CLI {
        Game::Game _model;
        View::CLI _view;

        std::string mainMenu();
        void startNewGame();
        void playGame(std::string &namePlayer1, std::string &namePlayer2);
        void showBoard() const;
        Position inputPosition(const std::string &message) const;
        std::string movePiece(const Position &from, const Position &to);

      public:
        CLI();

        void start();

      private:
        bool isEven(int x) const;
        bool isOdd(int x) const;
        bool isPieceCell(int i, int j, int gridRows, int gridCols) const;
        bool isColLabel(int gridNRow, int i, int j) const;
        bool isRowLabel(int gridNRow, int i, int j) const;
        bool isHorizontalBorder(int i, int j) const;
        bool isVerticalBorder(int i, int j) const;
        bool isCrossPoint(int i, int j) const;
        void evaluateEdnGame() const;
    };

} // namespace Controller

#endif // CONTROLLER_CLI_HPP
