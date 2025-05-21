
#pragma once

#include "GameBoard.h"
#include "Leaderboard.h"
#include <string>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void clear();
    void display();
    void renderMenu();
    void renderBoard(const GameBoard* board);
    void renderScore(int score);
    void renderPauseScreen();
    void renderGameOver(int finalScore);
    void renderHighScores(const Leaderboard* leaderboard);
    void renderMessage(const std::string& message);

private:
    static constexpr int SCREEN_WIDTH = 80;
    static constexpr int SCREEN_HEIGHT = 25;
    
    void centerText(const std::string& text, int row);
    void drawBorder();
    void drawHorizontalLine(int row);
    std::string padString(const std::string& str, int width) const;
    void clearScreen();
    
    std::vector<std::vector<char>> buffer;
};
    void drawHorizontalLine(int row);
