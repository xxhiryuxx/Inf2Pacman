/**
 * @file GameBoard.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 15:35
 */

#include "GameBoard.h"
#include "Ghost.h"
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

bool GameBoard::isWalkable(int x, int y) const {
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
    std::uniform_real_distribution<> spawnChance(0, 1);    // 10% chance to spawn a fruit in an empty space
    if (spawnChance(gen) < 0.1) {
        int x = disX(gen);
        int y = disY(gen);
        if (grid[y][x].getContent() == CellContent::EMPTY) {
            grid[y][x].setContent(CellContent::FRUIT);
        }
    }
}

void GameBoard::initialize() {
    clearBoard();
    loadDefaultLevel();
    spawnPowerPellets();
}

void GameBoard::clearBoard() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x].setContent(CellContent::EMPTY);
        }
    }
}

void GameBoard::loadDefaultLevel() {
    // Set up walls around the edges
    for (int x = 0; x < width; ++x) {
        grid[0][x].setContent(CellContent::WALL);
        grid[height-1][x].setContent(CellContent::WALL);
    }
    for (int y = 0; y < height; ++y) {
        grid[y][0].setContent(CellContent::WALL);
        grid[y][width-1].setContent(CellContent::WALL);
    }

    // Create a simple maze pattern
    for (int y = 3; y < height-3; y += 3) {
        for (int x = 3; x < width-3; x += 4) {
            // Create T-shaped wall patterns
            grid[y][x].setContent(CellContent::WALL);
            if (x < width-4) grid[y][x+1].setContent(CellContent::WALL);
            if (y > 3) grid[y-1][x].setContent(CellContent::WALL);
        }
    }    // Create ghost spawn area in the center
    int centerX = width / 2;
    int centerY = height / 2;
    for (int y = centerY-2; y <= centerY+1; y++) {
        for (int x = centerX-2; x <= centerX+2; x++) {
            grid[y][x].setContent(CellContent::EMPTY);
            // Add walls around spawn area
            if (y == centerY-2 || y == centerY+1) {
                grid[y][x].setContent(CellContent::WALL);
            }
            if (x == centerX-2 || x == centerX+2) {
                grid[y][x].setContent(CellContent::WALL);
            }
        }
    }
    // Add ghost exit
    grid[centerY-2][centerX].setContent(CellContent::EMPTY);    // Fill remaining empty spaces with coins
    for (int y = 1; y < height-1; ++y) {
        for (int x = 1; x < width-1; ++x) {
            if (grid[y][x].getContent() == CellContent::EMPTY) {
                grid[y][x].setContent(CellContent::COIN);
                remainingCoins++;
            }
        }
    }
}

void GameBoard::spawnPowerPellets() {
    // Place power pellets in strategic locations
    if (width > 2 && height > 2) {
        // Near each corner, but slightly inset for better gameplay
        grid[2][2].setContent(CellContent::POWER_PELLET);
        grid[2][width-3].setContent(CellContent::POWER_PELLET);
        grid[height-3][2].setContent(CellContent::POWER_PELLET);
        grid[height-3][width-3].setContent(CellContent::POWER_PELLET);
        
        // Optional middle power pellets for larger boards
        if (width >= 20 && height >= 20) {
            grid[2][width/2].setContent(CellContent::POWER_PELLET);
            grid[height-3][width/2].setContent(CellContent::POWER_PELLET);
        }
    }
}

void GameBoard::respawnGhost(Ghost* ghost) {
    // Ghost respawn positions in the center area
    static const std::vector<std::pair<int, int>> respawnPositions = {
        {width/2-1, height/2-1},
        {width/2+1, height/2-1},
        {width/2-1, height/2+1},
        {width/2+1, height/2+1}
    };
    
    static size_t nextPosition = 0;
    
    if (ghost && !respawnPositions.empty()) {
        ghost->setPosition(respawnPositions[nextPosition].first, 
                         respawnPositions[nextPosition].second);
        ghost->setScared(false);
        nextPosition = (nextPosition + 1) % respawnPositions.size();
    }
}
