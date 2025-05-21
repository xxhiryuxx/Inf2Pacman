#include "Renderer.h"
#include "GameBoard.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Renderer::Renderer() 
    : buffer(SCREEN_HEIGHT, std::vector<char>(SCREEN_WIDTH, ' '))
{
    clearScreen();
}

Renderer::~Renderer() {
    // No platform-specific cleanup needed
}

void Renderer::clear() {
    for (auto& row : buffer) {
        std::fill(row.begin(), row.end(), ' ');
    }
}

void Renderer::display() {
    clearScreen();
    for (const auto& row : buffer) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }
}

void Renderer::renderMenu() {
    clear();
    drawBorder();
    
    centerText("PAC-MAN", 5);
    centerText("Press ENTER to start", 10);
    centerText("Controls:", 14);
    centerText("W - Move Up", 15);
    centerText("S - Move Down", 16);
    centerText("A - Move Left", 17);
    centerText("D - Move Right", 18);
    centerText("P - Pause Game", 19);
}

void Renderer::renderBoard(const GameBoard* board) {
    if (!board) return;
    
    // Copy board state to buffer
    auto boardState = board->getState();
    int startRow = (SCREEN_HEIGHT - board->getHeight()) / 2;
    int startCol = (SCREEN_WIDTH - board->getWidth()) / 2;
    
    for (int y = 0; y < board->getHeight(); ++y) {
        for (int x = 0; x < board->getWidth(); ++x) {
            if (startRow + y < SCREEN_HEIGHT && startCol + x < SCREEN_WIDTH) {
                buffer[startRow + y][startCol + x] = boardState[y][x];
            }
        }
    }
}

void Renderer::renderPauseScreen() {
    centerText("GAME PAUSED", SCREEN_HEIGHT / 2 - 1);
    centerText("Press P to continue", SCREEN_HEIGHT / 2 + 1);
}

void Renderer::renderGameOver(int finalScore) {
    clear();
    drawBorder();
    
    centerText("GAME OVER", SCREEN_HEIGHT / 2 - 3);
    centerText("Final Score: " + std::to_string(finalScore), SCREEN_HEIGHT / 2 - 1);
    centerText("Press ENTER to return to menu", SCREEN_HEIGHT / 2 + 2);
}

void Renderer::renderMessage(const std::string& message) {
    centerText(message, SCREEN_HEIGHT - 1);
}

void Renderer::centerText(const std::string& text, int row) {
    if (row < 0 || row >= SCREEN_HEIGHT) return;
    
    int startCol = (SCREEN_WIDTH - text.length()) / 2;
    if (startCol < 0) startCol = 0;
    
    for (size_t i = 0; i < text.length(); ++i) {
        if (startCol + i < SCREEN_WIDTH) {
            buffer[row][startCol + i] = text[i];
        }
    }
}

void Renderer::drawBorder() {
    // Draw top and bottom borders
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        buffer[0][x] = '-';
        buffer[SCREEN_HEIGHT - 1][x] = '-';
    }
    
    // Draw side borders
    for (int y = 1; y < SCREEN_HEIGHT - 1; ++y) {
        buffer[y][0] = '|';
        buffer[y][SCREEN_WIDTH - 1] = '|';
    }
    
    // Draw corners
    buffer[0][0] = '+';
    buffer[0][SCREEN_WIDTH - 1] = '+';
    buffer[SCREEN_HEIGHT - 1][0] = '+';
    buffer[SCREEN_HEIGHT - 1][SCREEN_WIDTH - 1] = '+';
}

void Renderer::drawHorizontalLine(int row) {
    if (row < 0 || row >= SCREEN_HEIGHT) return;
    
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        buffer[row][x] = '-';
    }
}

std::string Renderer::padString(const std::string& str, int width) const {
    std::string result = str;
    while (result.length() < width) {
        result = " " + result + " ";
    }
    if (result.length() > width) {
        result = result.substr(0, width);
    }
    return result;
}

void Renderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}