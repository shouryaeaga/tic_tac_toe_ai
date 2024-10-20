//
// Created by shourya on 20/10/24.
//

#pragma once
#include "Game.h"

class AI {
public:
    static Game::Turn makeComputerTurn(bool maximisingPlayer1, std::array<std::array<std::string, 3>, 3> board);
private:
    static Game::ComputerResult resultReturn(bool maximisingPlayer1, const std::vector<Game::Winner> &evals, const std::vector<Game::Turn> &turns, bool firstRecursion);

    static Game::ComputerResult makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, bool maximisingPlayer1, bool firstRecursion);
};
