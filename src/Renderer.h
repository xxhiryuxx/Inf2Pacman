#pragma once

#include "GameBoard.h"
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
    void renderPauseScreen();
    void renderGameOver(int finalScore);
    void renderMessage(const std::string& message);
    void renderScore(int score);

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
