//
// Created by shourya on 17/10/24.
//

#pragma once
#include <array>
#include <string>

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

    Winner getWinner(std::array<std::array<std::string, 3>, 3> &board);
    Winner getWinner() {
        return getWinner(board);
    }

    Turn getPlayerTurn();
    void printBoard();
    Turn makeComputerTurn(int depth, bool maximisingPlayer1);

    Winner makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, int depth, bool maximisingPlayer1);

    bool updateBoard(Turn move, std::array<std::array<std::string,3>,3>& board);
    bool updateBoard(Turn move) {
        return updateBoard(move, board);
    }

};
