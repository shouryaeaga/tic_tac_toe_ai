//
// Created by shourya on 17/10/24.
//

#pragma once
#include <array>
#include <string>
#include <variant>

class Game {
public:
    Game();



    void Loop();
private:
    std::array<std::array<std::string, 3>, 3> board = {{{" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "}}};
    bool ai;

    struct Turn {
        int x;
        int y;
        bool player1;
    };

    enum Winner {
        PLAYER1 = 0,
        PLAYER2 = 1,
        DRAW = 2,
        INCOMPLETE = 3
    };

    using ComputerResult = std::variant<Turn, Winner>;

    Winner getWinner(std::array<std::array<std::string, 3>, 3> &board);
    Winner getWinner() {
        return getWinner(board);
    }

    Turn getPlayerTurn();
    void printBoard();
    Turn makeComputerTurn(bool maximisingPlayer1);

    Game::ComputerResult makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, bool maximisingPlayer1, bool firstRecursion);

    bool updateBoard(Turn move, std::array<std::array<std::string,3>,3>& board);
    bool updateBoard(Turn move) {
        return updateBoard(move, board);
    }

};
