#include "Game.h"
#include <iostream>
#include <cstdlib>

int main() {
    // Clear screen at start
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    // Display welcome message
    std::cout << "\n=== WELCOME TO PACMAN ===\n" << std::endl;

    std::cout << "\nPress Enter to start the game...";
    std::cin.get();

    // Initialize game with standard dimensions
    Game game;
    game.initializeGameBoard();
    game.startGame();

    return 0;
}
