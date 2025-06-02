// Implementation of Renderer: handles all drawing for the game
#include "Renderer.h"
#include "raylib.h"
#include <fstream>
#include "Constants.h"

// Draws the main game screen: map, player, ghosts, score, coins, highscore
void Renderer::drawGame(const GameBoard& board, const Player& pacman, const std::vector<Ghost>& ghosts, const Leaderboard& leaderboard) {
    BeginDrawing();
    ClearBackground(BLACK);
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int cols = board.getWidth();
    int rows = board.getHeight();

    int cellSize = std::min(screenWidth / cols, screenHeight / rows);
    int offsetX = (screenWidth - (cellSize * cols)) / 2;
    int offsetY = (screenHeight - (cellSize * rows)) / 2;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Rectangle rect = {
                static_cast<float>(offsetX + x * cellSize),
                static_cast<float>(offsetY + y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };
            if (board.field[y][x] == WALL) {
                DrawRectangleRec(rect, BLUE);
            } else {
                DrawRectangleRec(rect, BLACK);
                if (board.field[y][x] == COIN) {
                    DrawCircle(rect.x + cellSize / 2, rect.y + cellSize / 2, cellSize * 0.15f, YELLOW);
                } else if (board.field[y][x] == FRUIT) {
                    DrawCircle(rect.x + cellSize / 2, rect.y + cellSize / 2, cellSize * 0.25f, RED);
                }
            }
        }
    }
    // Draw ghosts
    for (const auto& g : ghosts) {
        DrawCircle(
            offsetX + g.x * cellSize + cellSize / 2,
            offsetY + g.y * cellSize + cellSize / 2,
            cellSize / 2 - 4, PURPLE);
    }
    // Draw Pacman
    DrawCircle(
        offsetX + pacman.x * cellSize + cellSize / 2,
        offsetY + pacman.y * cellSize + cellSize / 2,
        cellSize / 2 - 4, YELLOW);
    // Draw score, coins left, and highscore
    std::string scoreText = "Score: " + std::to_string(pacman.score);
    DrawText(scoreText.c_str(), 10, 10, 24, WHITE);
    std::string coinsText = "Coins left: " + std::to_string(board.coinsLeft);
    DrawText(coinsText.c_str(), 10, 40, 20, WHITE);
    std::string highscoreText = "Highscore: " + std::to_string(leaderboard.getHighscore()) + " (" + leaderboard.getHighscoreName() + ")";
    DrawText(highscoreText.c_str(), 10, 70, 20, WHITE);
    EndDrawing();
}

// Draws the start menu screen
void Renderer::drawStartMenu(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(BLACK);
    const char* titleText = "PAC-MAN";
    int titleWidth = MeasureText(titleText, 72);
    DrawText(titleText, screenWidth/2 - titleWidth/2, 120, 72, YELLOW);
    const char* subText = "Press [ENTER] to start";
    int subWidth = MeasureText(subText, 32);
    DrawText(subText, screenWidth/2 - subWidth/2, 250, 32, WHITE);
    const char* escText = "Exit with [ESC]";
    int escWidth = MeasureText(escText, 24);
    DrawText(escText, screenWidth/2 - escWidth/2, 300, 24, GRAY);
    const char* lbText = "View Leaderboard with [L]";
    int lbWidth = MeasureText(lbText, 24);
    DrawText(lbText, screenWidth/2 - lbWidth/2, 330, 24, GRAY);
    EndDrawing();
}

// Draws the pause menu screen
void Renderer::drawPauseMenu(int screenWidth, int screenHeight) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    const char* pauseText = "PAUSE";
    int pauseWidth = MeasureText(pauseText, 64);
    DrawText(pauseText, screenWidth/2 - pauseWidth/2, 180, 64, YELLOW);
    const char* continueText = "Continue with [P]";
    int contWidth = MeasureText(continueText, 32);
    DrawText(continueText, screenWidth/2 - contWidth/2, 250, 32, WHITE);
    const char* exitText = "Exit with [ESC]";
    int exitWidth = MeasureText(exitText, 25);
    int exitY = 300;
    DrawText(exitText, screenWidth/2 - exitWidth/2, exitY, 25, GRAY);
    const char* warnText = "Back to Main Menu with [Q] (Progress will be lost!)";
    int warnWidth = MeasureText(warnText, 25);
    int warnY = exitY + 40;
    DrawText(warnText, screenWidth/2 - warnWidth/2, warnY, 25, RED);
    EndDrawing();
}

// Draws the leaderboard screen (top 10 scores)
void Renderer::drawLeaderboard(int screenWidth, int screenHeight, const std::string& filename) {
    BeginDrawing();
    ClearBackground(BLACK);
    const char* lbTitle = "Leaderboard";
    int titleWidth = MeasureText(lbTitle, 48);
    DrawText(lbTitle, screenWidth/2 - titleWidth/2, 60, 48, YELLOW);
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name;
        int score;
        int line = 0;
        while (file >> name >> score && line < 10) {
            std::string entry = std::to_string(line + 1) + ". " + name + " - " + std::to_string(score);
            DrawText(entry.c_str(), screenWidth/2 - 150, 130 + line * 30, 24, WHITE);
            line++;
        }
        file.close();
    } else {
        DrawText("Unable to load leaderboard.", screenWidth/2 - 150, 130, 24, RED);
    }
    const char* backText = "Press [L] to return";
    int backWidth = MeasureText(backText, 20);
    DrawText(backText, screenWidth/2 - backWidth/2, screenHeight - 50, 20, GRAY);
    EndDrawing();
}

// Draws the game over screen (win/lose, score, highscore)
void Renderer::drawGameOver(int screenWidth, int screenHeight, int score, bool gameOver, const Leaderboard& leaderboard, const std::string& playerName, bool newHighscore) {
    BeginDrawing();
    ClearBackground(BLACK);
    const char* text = gameOver ? "Game Over!" : "You Won!";
    int textWidth = MeasureText(text, 48);
    DrawText(text, screenWidth/2 - textWidth/2, screenHeight/2 - 60, 48, gameOver ? RED : GREEN);
    std::string scoreText = "Score: " + std::to_string(score);
    int scoreWidth = MeasureText(scoreText.c_str(), 32);
    DrawText(scoreText.c_str(), screenWidth/2 - scoreWidth/2, screenHeight/2, 32, WHITE);
    if (newHighscore) {
        std::string hsText = "New Highscore!";
        int hsWidth = MeasureText(hsText.c_str(), 32);
        DrawText(hsText.c_str(), screenWidth/2 - hsWidth/2, screenHeight/2 + 50, 32, YELLOW);
    } else {
        std::string hsText = "Highscore: " + std::to_string(leaderboard.getHighscore()) + " (" + leaderboard.getHighscoreName() + ")";
        int hsWidth = MeasureText(hsText.c_str(), 24);
        DrawText(hsText.c_str(), screenWidth/2 - hsWidth/2, screenHeight/2 + 50, 24, WHITE);
    }
    const char* menuText = "Press [ENTER] for Main Menu";
    int menuWidth = MeasureText(menuText, 24);
    DrawText(menuText, screenWidth/2 - menuWidth/2, screenHeight/2 + 100, 24, GRAY);
    EndDrawing();
}
