#pragma once
#include "GameBoard.h"
#include "Player.h"
#include "Ghost.h"
#include "Leaderboard.h"
#include <vector>
#include <string>

class Renderer {
public:
    static void drawGame(const GameBoard& board, const Player& pacman, const std::vector<Ghost>& ghosts, const Leaderboard& leaderboard);
    static void drawStartMenu(int screenWidth, int screenHeight);
    static void drawPauseMenu(int screenWidth, int screenHeight);
    static void drawLeaderboard(int screenWidth, int screenHeight, const std::string& filename);
    static void drawGameOver(int screenWidth, int screenHeight, int score, bool gameOver, const Leaderboard& leaderboard, const std::string& playerName, bool newHighscore);
};
