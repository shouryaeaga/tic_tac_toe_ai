//
// Created by shourya on 17/10/24.
//

#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <array>
#include <utility>
#include <vector>

bool inputValidation(std::string message) {
    while (true) {
        std::cout << message << "\n";
        std::string input;
        std::getline(std::cin, input);
        if (input == "Y" || input == "y") {
            return true;
        }
        if (input == "N" || input == "n") {
            return false;
        }
        std::cout << "Invalid input\n";
    }

}

Game::Game() {
    srand(time(0));
    std::cout << "Welcome to Tic Tac Toe with AI\n";
    if (inputValidation("Do you want to play against AI (Y/N)? ")) {
        ai_player = rand() % 2 == 0 ? AI_PLAYER_1 : AI_PLAYER_2;
    } else {
        ai_player = NOT_PLAYING;
    }
}

Game::Winner Game::MakeTurn(bool player1, bool ai) {
    Turn playerTurn;
    if (ai) {
        playerTurn = makeComputerTurn(player1);
        playerTurn.player1 = player1;
    } else {
        playerTurn = getPlayerTurn();
        playerTurn.player1 = player1;
    }

    while (!updateBoard(playerTurn)) {
        std::cout << "Invalid move\n";
        playerTurn = getPlayerTurn();
        playerTurn.player1 = player1;
    }

    Winner winner = getWinner();

    printBoard();

    // CHECK WINNER
    if (winner == PLAYER1) {
        std::cout << "Player 1 wins\n";
        return PLAYER1;
    }
    if (winner == DRAW) {
        std::cout << "Draw\n";
        return DRAW;
    }
    if (winner == PLAYER2) {
        std::cout << "Player 2 wins\n";
        return PLAYER2;
    }
    return INCOMPLETE;
}

void Game::cleanBoard() {
    board = {
        {
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)},
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)},
            {PieceToString(EMPTY), PieceToString(EMPTY), PieceToString(EMPTY)}
        }
    };
}

void Game::Loop() {
    while (true) {
        if (ai_player == AI_PLAYER_1)
            std::cout << "AI is thinking....\n";


        if (MakeTurn(true, ai_player == AI_PLAYER_1) != INCOMPLETE) {
            if (inputValidation("Do you want to play again (Y/N)? ")) {
                cleanBoard();
                if (inputValidation("Do you want to play against AI (Y/N)? "))
                    ai_player = rand() % 2 == 0 ? AI_PLAYER_1 : AI_PLAYER_2;
                else
                    ai_player = NOT_PLAYING;

            } else {
                break;
            }
        };

        if (ai_player == AI_PLAYER_2)
            std::cout << "AI is thinking....\n";


        if (MakeTurn(false, ai_player==AI_PLAYER_2) != INCOMPLETE) {
            if (inputValidation("Do you want to play again (Y/N)? ")) {
                cleanBoard();
                if (inputValidation("Do you want to play against AI (Y/N)? "))
                    ai_player = rand() % 2 == 0 ? AI_PLAYER_1 : AI_PLAYER_2;
                else
                    ai_player = NOT_PLAYING;
            } else {
                break;
            }
        };
    }
}

Game::Turn Game::getPlayerTurn() {
    bool continueWhile = true;
    while (continueWhile) {
        // Get input
        std::cout << "Enter your turn in the format x,y: ";
        std::string input;
        std::getline(std::cin, input);

        // Check if all of them are digits
        if (
            !std::ranges::all_of(
                input,
                [](const char c){ return std::isdigit(c) || c == ','; })
            || input.length() != 3
                )
        {
            std::cout << "Invalid input! Please try again!\n";
            continue;
        }

        // Get x and y
        std::stringstream stream(input);
        int xy[2];
        int counter = 0;
        for (std::string xyInput; std::getline(stream, xyInput, ',');) {
            if (std::stoi(xyInput) < 1 || std::stoi(xyInput) > 3) {
                std::cout << "Invalid input! Please try again!\n";
                continueWhile = false;
                break;
            } else {
                xy[counter] = std::stoi(xyInput);
            }
            counter++;
        }

        if (!continueWhile) {
            continueWhile = true;
            continue;
        }

        return Turn(xy[0], xy[1]);
    }
}

void Game::printBoard() {
    std::cout << "\n";
    std::cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
    std::cout << "_ _ _ _ _\n";

    std::cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
    std::cout << "_ _ _ _ _\n";

    std::cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n";

}

Game::Turn Game::makeComputerTurn(const bool maximisingPlayer1) {
    ComputerResult result = makeComputerTurnHelper(board, maximisingPlayer1, true);
    return std::get<Turn>(result);
}

Game::ComputerResult Game::makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, bool maximisingPlayer1, bool firstRecursion) {
    // If depth is 0, or game is over, return the evaluation of position
    if (getWinner(board) != INCOMPLETE) {
        return getWinner(board);
    }

    // Get all possible moves
    std::vector<std::array<int, 2>> possibleMoves;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j] == PieceToString(EMPTY)) {
                possibleMoves.push_back({j+1, 3-i});
            }
        }
    }

    std::vector<Winner> evals;
    std::vector<Turn> turns;

    // Now recursively call each one
    for (int i = 0; i < possibleMoves.size(); i++) {
        Turn nextTurn {
            possibleMoves[i][0],
            possibleMoves[i][1],
            maximisingPlayer1
        };
        std::array<std::array<std::string,3>,3> newBoard = board;
        updateBoard(nextTurn, newBoard);
        ComputerResult eval = makeComputerTurnHelper(newBoard, !maximisingPlayer1, false);
        if (std::holds_alternative<Winner>(eval))
            evals.push_back(std::get<Winner>(eval));

        turns.push_back(nextTurn);
    }

    // Return the best eval for the maximising player
    if (maximisingPlayer1) {
        // Return the drawing position or winning position if there is no drawing position.
        int wins = 0;
        int draws = 0;
        int losses = 0;

        // Make a vector of winning moves and drawing moves and losing moves
        std::vector<Turn> winningTurns;
        std::vector<Turn> drawingTurns;
        std::vector<Turn> losingTurns;

        for (int i = 0; i < evals.size(); i++) {
            if (evals[i] == PLAYER1) {
                wins += 1;
                winningTurns.push_back(turns[i]);
            } else if (evals[i] == DRAW) {
                draws += 1;
                drawingTurns.push_back(turns[i]);
            } else if (evals[i] == PLAYER2) {
                losses += 1;
                losingTurns.push_back(turns[i]);
            }
        }
        if (wins > 0) {
            if (firstRecursion) {
                return winningTurns[0];
            }
            return PLAYER1;
        }
        if (draws > 0) {
            if (firstRecursion) {
                return drawingTurns[0];
            }
            return DRAW;
        }
        if (losses > 0) {
            if (firstRecursion) {
                return losingTurns[0];
            }
            return PLAYER2;
        }
    } else {
        // Return the drawing position or winning position if there is no drawing position.
        int wins = 0;
        int draws = 0;
        int losses = 0;

        // Make a vector of winning moves and drawing moves and losing moves
        std::vector<Turn> winningTurns;
        std::vector<Turn> drawingTurns;
        std::vector<Turn> losingTurns;

        for (int i = 0; i < evals.size(); i++) {
            if (evals[i] == PLAYER2) {
                wins += 1;
                winningTurns.push_back(turns[i]);
            } else if (evals[i] == DRAW) {
                draws += 1;
                drawingTurns.push_back(turns[i]);
            } else if (evals[i] == PLAYER1) {
                losses += 1;
                losingTurns.push_back(turns[i]);
            }
        }
        if (wins > 0) {
            if (firstRecursion) {
                return winningTurns[0];
            }
            return PLAYER2;
        }
        if (draws > 0) {
            if (firstRecursion) {
                return drawingTurns[0];
            }
            return DRAW;
        }
        if (losses > 0) {
            if (firstRecursion) {
                return losingTurns[0];
            }
            return PLAYER1;
        }
    }
}

bool Game::updateBoard(Turn move, std::array<std::array<std::string,3>,3>& board) {
    auto [x, y, player1] = move;
    // Update board
    // Check if the coordinates exist
    if (board[3-y][x-1] == PieceToString(EMPTY)) {
        if (player1) {
            board[3-y][x-1] = PieceToString(X);
            return true;
        }
        board[3-y][x-1] = PieceToString(O);
        return true;
    }
    return false;
}

Game::Winner Game::getWinner(std::array<std::array<std::string, 3>, 3>& board) {
    std::array<std::array<int, 2>, 3> winningCombinations;
    Winner winner = DRAW;

    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != PieceToString(EMPTY)) {
            // Update board to have dashes running through them
            winningCombinations[0] = {1, i+1};
            winningCombinations[1] = {2, i+1};
            winningCombinations[2] = {3, i+1};
            if (board[i][0] == PieceToString(X) || board[i][0] == PieceToString(X_WIN))
                winner = PLAYER1;
            else
                winner = PLAYER2;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != PieceToString(EMPTY)) {
            winningCombinations[0] = {i+1, 1};
            winningCombinations[1] = {i+1, 2};
            winningCombinations[2] = {i+1, 3};

            if (board[0][i] == PieceToString(X) || board[0][i] == PieceToString(X_WIN)) {
                winner = PLAYER1;
            } else {
                winner = PLAYER2;
            }
        }
    }

    // Check diagonals top right and bottom right
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != PieceToString(EMPTY)) {
        winningCombinations[0] = {1, 1};
        winningCombinations[1] = {2, 2};
        winningCombinations[2] = {3, 3};
        if (board[0][0] == PieceToString(X) || board[0][0] == PieceToString(X_WIN))
            winner = PLAYER1;
        else {
            winner = PLAYER2;
        }
    }

    if (board[2][0] == board[1][1] && board[2][0] == board[0][2] && board[2][0] != PieceToString(EMPTY)) {
        winningCombinations[0] = {1, 3};
        winningCombinations[1] = {2, 2};
        winningCombinations[2] = {3, 1};
        if (board[2][0] == PieceToString(X) || board[2][0] == PieceToString(X_WIN))
            winner = PLAYER1;
        else {
            winner = PLAYER2;
        }
    }

    // Check if it is a draw
    if (winner != PLAYER1 && winner != PLAYER2) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == PieceToString(EMPTY)) {
                    winner = INCOMPLETE;
                }
            }
        }
    }

    if (winner != INCOMPLETE && winner != DRAW) {
        std::string replacement = PieceToString(EMPTY);
        if (winner == PLAYER1) {
            replacement = PieceToString(X_WIN);
        } else {
            replacement = PieceToString(O_WIN);
        }
        // Set winning combinations to have a dash
        for (int x = 1; x <= 3; x++) {
            for (int y = 1; y <= 3; y++) {
                for (int i = 0; i < 3; i++) {
                    if (winningCombinations[i][0] == x && winningCombinations[i][1] == y)
                        board[y-1][x-1] = replacement;
                }
            }
        }
    }

    return winner;

}
