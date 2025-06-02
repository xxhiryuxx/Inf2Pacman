#pragma once
#include <vector>
#include "Constants.h"

// GameBoard class: represents the game map, coins, walls, and fruit
class GameBoard {
public:
    std::vector<std::vector<Cell>> field; // 2D grid of cells (WALL, COIN, EMPTY, etc.)
    int coinsLeft;                        // Number of coins left on the board
    void generateRandomMap();
    const std::vector<std::pair<int, int>>& getGhostStartPositions() const;
    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }
};
