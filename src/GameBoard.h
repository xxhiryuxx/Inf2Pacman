#pragma once

#include <vector>
#include "GridPoint.h"
#include "PacMan.h"
#include "Ghost.h"

class GameBoard {
public:
    static constexpr int WIDTH = 10;
    static constexpr int HEIGHT = 10;

    GameBoard();
    ~GameBoard();

    // Essential gameplay functions
    void update();  // Update game state
    GridPoint* getGridPoint(int x, int y);  // Get grid cell for movement/collision
    PacMan* getPacMan() const;  // Get PacMan instance
    std::vector<Ghost*> getGhosts() const;  // Get ghosts for collision
    int getRemainingCollectibles() const;  // Check win condition

private:
    std::vector<std::vector<GridPoint>> grid;
    PacMan* pacman;
    std::vector<Ghost*> ghosts;
};