//
// Created by shourya on 20/10/24.
//

#include "AI.h"


Game::Turn AI::makeComputerTurn(const bool maximisingPlayer1, std::array<std::array<std::string, 3>, 3> board) {
    Game::ComputerResult result = makeComputerTurnHelper(board, maximisingPlayer1, true);
    return std::get<Game::Turn>(result);
}

Game::ComputerResult AI::resultReturn(bool maximisingPlayer1, const std::vector<Game::Winner> &evals, const std::vector<Game::Turn>& turns, bool firstRecursion) {
    // Return the drawing position or winning position if there is no drawing position.
    int wins = 0;
    int draws = 0;
    int losses = 0;

    // Make a vector of winning moves and drawing moves and losing moves
    std::vector<Game::Turn> winningTurns;
    std::vector<Game::Turn> drawingTurns;
    std::vector<Game::Turn> losingTurns;

    Game::Winner losing = maximisingPlayer1 ? Game::PLAYER2 : Game::PLAYER1;
    Game::Winner winning = maximisingPlayer1 ? Game::PLAYER1 : Game::PLAYER2;

    for (int i = 0; i < evals.size(); i++) {
        if (evals[i] == winning) {
            wins += 1;
            winningTurns.push_back(turns[i]);
        } else if (evals[i] == Game::Winner::DRAW) {
            draws += 1;
            drawingTurns.push_back(turns[i]);
        } else if (evals[i] == losing) {
            losses += 1;
            losingTurns.push_back(turns[i]);
        }
    }
    if (wins > 0) {
        if (firstRecursion) {
            unsigned int middle = winningTurns.size() / 2;
            return winningTurns[middle];
        }
        return winning;
    }
    if (draws > 0) {
        if (firstRecursion) {
            unsigned int middle = drawingTurns.size() / 2;
            return drawingTurns[middle];
        }
        return Game::Winner::DRAW;
    }
    if (losses > 0) {
        if (firstRecursion) {
            unsigned int middle = losingTurns.size() / 2;
            return losingTurns[middle];
        }
        return losing;
    }
}

Game::ComputerResult AI::makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, bool maximisingPlayer1, bool firstRecursion) {
    // If depth is 0, or game is over, return the evaluation of position
    if (Game::getWinner(board) != Game::Winner::INCOMPLETE) {
        return Game::getWinner(board);
    }

    // Get all possible moves
    std::vector<std::array<int, 2>> possibleMoves;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j] == Game::PieceToString(Game::PieceType::EMPTY)) {
                possibleMoves.push_back({j+1, 3-i});
            }
        }
    }

    std::vector<Game::Winner> evals;
    std::vector<Game::Turn> turns;

    // Now recursively call each one
    for (int i = 0; i < possibleMoves.size(); i++) {
        Game::Turn nextTurn {
            possibleMoves[i][0],
            possibleMoves[i][1],
            maximisingPlayer1
        };
        std::array<std::array<std::string,3>,3> newBoard = board;
        Game::updateBoard(nextTurn, newBoard);
        Game::ComputerResult eval = makeComputerTurnHelper(newBoard, !maximisingPlayer1, false);
        if (std::holds_alternative<Game::Winner>(eval))
            evals.push_back(std::get<Game::Winner>(eval));

        turns.push_back(nextTurn);
    }

    return resultReturn(maximisingPlayer1, evals, turns, firstRecursion);
}

