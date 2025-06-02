#pragma once
#include "raylib.h"
#include "GameBoard.h"
#include "Player.h"
#include "Ghost.h"
#include "Leaderboard.h"
#include "MazeCell.h"
#include "Constants.h"
#include <vector>
#include <string>




// All possible game states
enum GameState {
    STATE_START_MENU,
    STATE_ENTER_NAME,
    STATE_LEADERBOARD,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
};

class Game {
public:
    GameBoard board;                // The game board
    Player pacman;                  // The player character
    std::vector<Ghost> ghosts;      // All ghosts
    Leaderboard leaderboard;        // Leaderboard manager
    std::string playerName;         // Current player's name
    bool gameOver;                  // Is the game over?
    GameState state;                // Current game state

    // Constructs a new game and initializes all components
    Game();
    // Handles player name input at the start of the game
    bool getPlayerName();
    // Handles Pac-Man movement and coin/fruit collection
    void movePacman();
    // Handles ghost movement logic
    void moveGhosts();
    // Main game loop and state management
    void run();
    // Checks for collisions between Pac-Man and ghosts
    void checkCollision();
    // Spawns a fruit at a random empty location
    void spawnFruit();
    // Waits for a key to be released before continuing
    void waitForKeyRelease(int key);
};
