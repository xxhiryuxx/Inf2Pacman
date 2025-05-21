/**
 * @file GameBoard.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 15:35
 */

#pragma once
#include "GridPoint.h"
#include <vector>
#include <memory>

// Forward declarations
class PacMan;
class Ghost;

class GameBoard {
public:
    GameBoard(int width, int height);
    ~GameBoard();    
    void initialize();
    void draw();
    GridPoint& getGridPoint(int x, int y);
    const GridPoint& getGridPoint(int x, int y) const;
    GridPoint& getGridPoint(const Position& pos);
    const GridPoint& getGridPoint(const Position& pos) const;
    bool isWalkable(int x, int y) const;
    void updateRandomFruit();
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getPointsAt(int x, int y) const;  // Returns point value of collectible at position
    
    // Game state queries
    int getRemainingCoins() const;
    PacMan* getPacMan() const { return pacman; }
    std::vector<Ghost*>& getGhosts() { return ghosts; }

    // Ghost spawn/respawn handling
    void respawnGhost(Ghost* ghost);
    void spawnPowerPellets();

private:
    int width;
    int height;
    std::vector<std::vector<GridPoint>> grid;
    PacMan* pacman;
    std::vector<Ghost*> ghosts;
    int remainingCoins;

    void loadDefaultLevel();    
    void clearBoard();
};