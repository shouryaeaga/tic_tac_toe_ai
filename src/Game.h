//
// Created by shourya on 17/10/24.
//

#pragma once
#include <array>
#include <string>
#include <variant>
#include <vector>

class Game {
public:
    Game();

    void Loop();

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

    static bool updateBoard(Turn move, std::array<std::array<std::string, 3>, 3> &board);

    static Winner getWinner(std::array<std::array<std::string, 3>, 3> &board);

    enum PieceType {
        X,
        O,
        X_WIN,
        O_WIN,
        EMPTY
    };

    static std::string PieceToString(PieceType type) {
        switch (type) {
            case X: return "X";
            case O: return "O";
            case X_WIN: return "X̶";
            case O_WIN: return "O̶";
            case EMPTY: return " ";
        }
    }
private:
    std::array<std::array<std::string, 3>, 3> board = {
        {
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)},
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)},
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)}
        }};
    enum AI_PLAYER {
        AI_PLAYER_1 = 1,
        AI_PLAYER_2 = 2,
        NOT_PLAYING = 0
    };
    AI_PLAYER ai_player;

    Winner getWinner() {
        return getWinner(board);
    }
    Winner MakeTurn(bool player1, bool ai);

    void cleanBoard();

    Turn getPlayerTurn();
    void printBoard();

    bool updateBoard(Turn move) {
        return updateBoard(move, board);
    }

};
