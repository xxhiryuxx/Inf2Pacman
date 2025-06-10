// Main entry point for Pacman game
#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include "Entity.h"
#include "Player.h"
#include "Ghost.h"
#include "GameBoard.h"
#include "Leaderboard.h"
#include "MazeCell.h"
#include "Constants.h"
#include "Game.h"

// Entry point: seeds RNG, creates and runs the game
int main() {
    srand((unsigned int)time(nullptr)); // Seed random number generator
    Game game;                         // Create game instance
    game.run();                        // Start the game loop
    return 0;
}
