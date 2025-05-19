/**
 * @file GameBoard.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 15:35
 */

#include "GameBoard.h"
#include <random>
#include <iostream>

GameBoard::GameBoard(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<GridPoint>(width));
}

GameBoard::~GameBoard() {
    // No need to manually delete anything - grid points are not pointers anymore
}

void GameBoard::draw() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x].draw();
        }
        std::cout << std::endl;
    }
}

void GameBoard::setElement(int x, int y, CellContent content) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x].setContent(content);
    }
}

CellContent GameBoard::getElement(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x].getContent();
    }
    return CellContent::WALL;  // Return WALL for out of bounds (not walkable)
}

bool GameBoard::isWalkable(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x].isWalkable();
    }
    return false;
}

void GameBoard::updateRandomFruit() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);
    std::uniform_real_distribution<> spawnChance(0, 1);

    // 10% chance to spawn a fruit in an empty space
    if (spawnChance(gen) < 0.1) {
        int x = disX(gen);
        int y = disY(gen);
        if (grid[y][x].isEmpty()) {
            grid[y][x].setContent(CellContent::FRUIT);
        }
    }
}
