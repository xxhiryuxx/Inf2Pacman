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
    GameBoard board;
    Player pacman;
    std::vector<Ghost> ghosts;
    Leaderboard leaderboard;
    std::string playerName;
    bool gameOver;
    GameState state;

    Game();
    void shuffleDirections(int dx[], int dy[], int n);
    void dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze);
    void generateRandomMap();
    bool getPlayerName();
    void movePacman();
    void moveGhosts();
    void run();
    void checkCollision();
    void spawnFruit();
    void waitForKeyRelease(int key);
};
