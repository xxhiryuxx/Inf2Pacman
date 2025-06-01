
#pragma once
#include <vector>
#include "Constants.h"
// GameBoard class: represents the game map, coins, walls, and fruit
class GameBoard {
public:
    std::vector<std::vector<Cell>> field; // 2D grid of cells (WALL, COIN, EMPTY, etc.)
    int coinsLeft;                        // Number of coins left on the board
    bool fruitPresent;                    // Is a fruit currently present?
    int fruitX, fruitY;                   // Fruit position
    void generateRandomMap();
};
