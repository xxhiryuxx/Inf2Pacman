#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class GameBoard;
class Player;
class Ghost;
class Leaderboard;

class Renderer {
public:
    static void init();
    static void unload();
    static void drawGame(const GameBoard& board, const Player& pacman, const std::vector<Ghost>& ghosts, const Leaderboard& leaderboard);
    static void drawStartMenu(int screenWidth, int screenHeight);
    static void drawPauseMenu(int screenWidth, int screenHeight);
    static void drawLeaderboard(int screenWidth, int screenHeight, const std::string& filename);
    static void drawGameOver(int screenWidth, int screenHeight, int score, bool gameOver, const Leaderboard& leaderboard, const std::string& playerName, bool newHighscore);
private:
    static Texture2D pacmanTexture;
};
