// Renderer class: handles all drawing and rendering for the game
#pragma once
#include "GameBoard.h"
#include "Player.h"
#include "Ghost.h"
#include "Leaderboard.h"
#include <vector>
#include <string>

class Renderer {
public:
    // Draws the main game screen (map, player, ghosts, score, etc.)
    static void drawGame(const GameBoard& board, const Player& pacman, const std::vector<Ghost>& ghosts, const Leaderboard& leaderboard);
    // Draws the start menu
    static void drawStartMenu(int screenWidth, int screenHeight);
    // Draws the pause menu
    static void drawPauseMenu(int screenWidth, int screenHeight);
    // Draws the leaderboard screen
    static void drawLeaderboard(int screenWidth, int screenHeight, const std::string& filename);
    // Draws the game over screen
    static void drawGameOver(int screenWidth, int screenHeight, int score, bool gameOver, const Leaderboard& leaderboard, const std::string& playerName, bool newHighscore);
};
