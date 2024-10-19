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

Game::Game() {
    std::cout << "Welcome to Tic Tac Toe with AI\n";
    bool initialised = false;
    // Check if player wants ai or just 2 players
    while (!initialised) {
        std::cout << "Do you want to play against AI (Y/N)? ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "Y" || input == "y") {
            ai = true;
            initialised = true;
        }
        else if (input == "N" || input == "n") {
            ai = false;
            initialised = true;
        }
        else
            std::cout << "Invalid Input\n";
    }

}

void Game::Loop() {
    while (true) {
        Turn player1Turn = getPlayerTurn();
        player1Turn.player1 = true;

        while (!updateBoard(player1Turn)) {
            std::cout << "Invalid move\n";
            player1Turn = getPlayerTurn();
            player1Turn.player1 = true;
        }

        Winner winner = getWinner();

        // CHECK WINNER
        if (winner == Winner::PLAYER1) {
            printBoard();
            std::cout << "Player 1 wins\n";
            break;
        }
        if (winner == Winner::DRAW) {
            printBoard();
            std::cout << "Draw\n";
            break;
        }

        printBoard();

        makeComputerTurn(9, false);

        // TODO: make ai move the pieces

        Turn player2Turn = getPlayerTurn();
        player2Turn.player1 = false;


        while (!updateBoard(player2Turn)) {
            std::cout << "Invalid move\n";
            player2Turn = getPlayerTurn();
            player2Turn.player1 = false;
        }

        winner = getWinner();

        // CHECK WINNER
        if (winner == Winner::PLAYER2) {
            printBoard();
            std::cout << "Player 2 wins\n";
            break;
        }
        if (winner == Winner::DRAW) {
            printBoard();
            std::cout << "Draw\n";
            break;
        }

        makeComputerTurn(9, true);

        printBoard();
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

Game::Turn Game::makeComputerTurn(const int depth, const bool maximisingPlayer1) {
    makeComputerTurnHelper(board, depth, maximisingPlayer1);
    Turn turn {
        1,
        1,
        true
    };
    return turn;
}

Game::Winner Game::makeComputerTurnHelper(std::array<std::array<std::string, 3>, 3> board, const int depth, bool maximisingPlayer1) {
    // If depth is 0, or game is over, return the evaluation of position
    if (depth == 0 || getWinner(board) != INCOMPLETE) {
        return getWinner(board);
    }

    // Get all possible moves
    std::vector<std::array<int, 2>> possibleMoves;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j] == " ") {
                possibleMoves.push_back({j+1, 3-i});
            }
        }
    }

    std::vector<Winner> evals;

    // Now recursively call each one
    for (int i = 0; i < possibleMoves.size(); i++) {
        Turn nextTurn {
            possibleMoves[i][0],
            possibleMoves[i][1],
            maximisingPlayer1
        };
        std::array<std::array<std::string,3>,3> newBoard = board;
        updateBoard(nextTurn, newBoard);
        Winner eval = makeComputerTurnHelper(newBoard, depth-1, !maximisingPlayer1);
        evals.push_back(eval);
    }

    // print out all the evals
    if (depth == 9) {
        for (int i = 0; i < evals.size(); i++) {
            std::cout << "For move (" << possibleMoves[i][0] << ", " << possibleMoves[i][1] << "):\n";
            std::cout << evals[i] << "\n";
        }
    }

    // Return the best eval for the maximising player
    if (maximisingPlayer1) {
        // Return the drawing position or winning position if there is no drawing position.
        int wins = 0;
        int draws = 0;
        int losses = 0;
        for (Winner eval : evals) {
            if (eval == PLAYER1) {
                wins += 1;
            } else if (eval == DRAW) {
                draws += 1;
            } else if (eval == PLAYER2) {
                losses += 1;
            }
        }
        if (wins > 0) {
            return PLAYER1;
        }
        if (draws > 0) {
            return DRAW;
        }
        if (losses > 0) {
            return PLAYER2;
        }
    } else {
        // Return the drawing position or winning position if there is no drawing position.
        int wins = 0;
        int draws = 0;
        int losses = 0;
        for (Winner eval : evals) {
            if (eval == PLAYER1) {
                losses += 1;
            } else if (eval == DRAW) {
                draws += 1;
            } else if (eval == PLAYER2) {
                wins += 1;
            }
        }
        if (wins > 0) {
            return PLAYER2;
        }
        if (draws > 0) {
            return DRAW;
        }
        if (losses > 0) {
            return PLAYER1;
        }
    }
    return INCOMPLETE;
}

bool Game::updateBoard(Turn move, std::array<std::array<std::string,3>,3>& board) {
    auto [x, y, player1] = move;
    // Update board
    // Check if the coordinates exist
    if (board[3-y][x-1] == " ") {
        if (player1) {
            board[3-y][x-1] = 'X';
            return true;
        }
        board[3-y][x-1] = 'O';
        return true;
    }
    return false;
}

Game::Winner Game::getWinner(std::array<std::array<std::string, 3>, 3>& board) {
    std::array<std::array<int, 2>, 3> winningCombinations;
    Winner winner = DRAW;

    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != " ") {
            // Update board to have dashes running through them
            winningCombinations[0] = {1, i+1};
            winningCombinations[1] = {2, i+1};
            winningCombinations[2] = {3, i+1};
            if (board[i][0] == "X" || board[i][0] == "X̶")
                winner = PLAYER1;
            else
                winner = PLAYER2;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != " ") {
            winningCombinations[0] = {i+1, 1};
            winningCombinations[1] = {i+1, 2};
            winningCombinations[2] = {i+1, 3};

            if (board[0][i] == "X" || board[0][i] == "X̶") {
                winner = PLAYER1;
            } else {
                winner = PLAYER2;
            }
        }
    }

    // Check diagonals top right and bottom right
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != " ") {
        winningCombinations[0] = {1, 1};
        winningCombinations[1] = {2, 2};
        winningCombinations[2] = {3, 3};
        if (board[0][0] == "X" || board[0][0] == "X̶")
            winner = PLAYER1;
        else {
            winner = PLAYER2;
        }
    }

    if (board[2][0] == board[1][1] && board[2][0] == board[0][2] && board[2][0] != " ") {
        winningCombinations[0] = {1, 3};
        winningCombinations[1] = {2, 2};
        winningCombinations[2] = {3, 1};
        if (board[2][0] == "X" || board[2][0] == "X̶")
            winner = PLAYER1;
        else {
            winner = PLAYER2;
        }
    }

    // Check if it is a draw
    if (winner != PLAYER1 && winner != PLAYER2) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == " ") {
                    winner = INCOMPLETE;
                }
            }
        }
    }

    if (winner != INCOMPLETE && winner != DRAW) {
        std::string replacement = " ";
        if (winner == PLAYER1) {
            replacement = "X̶";
        } else {
            replacement = "O̶";
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
