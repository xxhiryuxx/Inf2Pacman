#include "Ghost.h"
#include "GameBoard.h"
#include "Player.h"
#include "Constants.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>

// Ghost constructor initializes the ghost's starting position, color, speed, and move counter
Ghost::Ghost(int startX, int startY, Color color)
    : Entity(startX, startY), 
    startX(startX), 
    startY(startY), 
    color(color) {
    speed = 30;       // Lower numbers for faster ghosts
    moveCounter = 0;
}

/* Function that resets the ghost's position and direction, but no usage in the current code
// Set ghosts to their starting positions and set initial direction
void Ghost::resetToStart() {
    x = startX;
    y = startY;
    dx = 0;
    dy = -1;
    moveCounter = 0;
}*/

// Update ghost position based on Pacman's position and game board state
void Ghost::update(const GameBoard& board, const Player& pacman) {
    moveCounter++;
    if (moveCounter < speed) return;
    moveCounter = 0;
    int bestDx = 0, bestDy = 0;
    int minDist = INT_MAX;
    bool foundValidDirection = false;

    // Define possible movement directions (Up, Right, Down, Left)
    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    // 1. Check every possible direction apart from Reverse
    for (auto [ndx, ndy] : directions) {
        if (-ndx == dx && -ndy == dy) continue; // Exclude reverse direction
        
        // Calculate new position
        int nx = x + ndx;
        int ny = y + ndy;
        if (nx >= 0 && nx < board.getWidth() && 
            ny >= 0 && ny < board.getHeight() && 
            board.field[ny][nx] != WALL) {
            
            // Calculate distance to Pacman and choose the shortest valid direction
            int dist = abs(pacman.x - nx) + abs(pacman.y - ny);
            if (dist < minDist || (dist == minDist && rand() % 2 == 0)) {
                minDist = dist;
                bestDx = ndx;
                bestDy = ndy;
                foundValidDirection = true;
            }
        }
    }

    // 2. Incase no valid direction found, try to reverse
    if (!foundValidDirection) {
        int reverseDx = -dx;
        int reverseDy = -dy;
        int nx = x + reverseDx;
        int ny = y + reverseDy;
        if (nx >= 0 && nx < board.getWidth() && 
            ny >= 0 && ny < board.getHeight() && 
            board.field[ny][nx] != WALL) {
            bestDx = reverseDx;
            bestDy = reverseDy;
            foundValidDirection = true;
        }
    }

    // 3. Only move if a valid direction was found
    if (foundValidDirection) {
        dx = bestDx;
        dy = bestDy;
        x += dx;
        y += dy;
    }
}
