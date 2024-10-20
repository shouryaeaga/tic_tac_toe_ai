//
// Created by shourya on 17/10/24.
//

#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <array>

#include "AI.h"

bool inputValidation(const std::string& message) {
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
    srand(time(nullptr));
    std::cout << "Welcome to Tic Tac Toe with AI\n";
    if (inputValidation("Do you want to play against AI (Y/N)? ")) {
        ai_player = rand() % 2 == 0 ? AI_PLAYER_1 : AI_PLAYER_2;
    } else {
        ai_player = NOT_PLAYING;
    }
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

Game::Winner Game::MakeTurn(bool player1, bool ai) {
    Turn playerTurn;
    if (ai) {
        playerTurn = AI::makeComputerTurn(player1, board);
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


Game::Turn Game::getPlayerTurn() {
    bool continueWhile = true;
    int xy[2];
    while (continueWhile) {
        bool success = true;
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
            success = false;
        }

        // Get x and y
        std::stringstream stream(input);

        int counter = 0;
        for (std::string xyInput; std::getline(stream, xyInput, ',');) {
            if (std::stoi(xyInput) < 1 || std::stoi(xyInput) > 3) {
                std::cout << "Invalid input! Please try again!\n";
                success = false;
                break;
            }
            xy[counter] = std::stoi(xyInput);
            counter++;
        }

        if (!success) {
            continueWhile = true;
            continue;
        }
        continueWhile = false;
    }
    return Turn(xy[0], xy[1]);
}

void Game::printBoard() {
    std::cout << "\n";
    std::cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
    std::cout << "_ _ _ _ _\n";

    std::cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
    std::cout << "_ _ _ _ _\n";

    std::cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n";

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
