// Game class declaration and main game state definitions
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

// All possible game states for the Pacman game
enum GameState {
    STATE_START_MENU,    // Main menu
    STATE_ENTER_NAME,    // Enter player name
    STATE_LEADERBOARD,  // Show leaderboard
    STATE_PLAYING,      // Game is running
    STATE_PAUSED,       // Game is paused
    STATE_GAME_OVER     // Game over screen
};

// Main Game class: manages all game logic and state
class Game {
public:
    GameBoard board;                // The game board (map, coins, walls, etc.)
    Player pacman;                  // The player character
    std::vector<Ghost> ghosts;      // All ghosts in the game
    Leaderboard leaderboard;        // Highscore/leaderboard manager
    std::string playerName;         // Current player's name
    bool gameOver;                  // True if the game is over
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

    void shuffleDirections(int dx[], int dy[], int n); // Shuffle directions for maze generation
    void dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze); // DFS for maze
};
