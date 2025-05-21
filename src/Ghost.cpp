/**
 * @file Ghost.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "Ghost.h"
#include "GameBoard.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

Ghost::Ghost(GameBoard* board) 
    : gameBoard(board), lastDirection(Direction::RIGHT), scared(false) {}

Ghost::~Ghost() {}

void Ghost::moveAutomatically() {
    if (!gameBoard) return;
    
    Direction nextDir = chooseNextDirection();
    moveDirection(nextDir);
    lastDirection = nextDir;

    // Calculate new position
    int newX = getX();
    int newY = getY();
    switch(nextDir) {
        case Direction::UP: newY--; break;
        case Direction::DOWN: newY++; break;
        case Direction::LEFT: newX--; break;
        case Direction::RIGHT: newX++; break;
    }
    
    if (gameBoard->isWalkable(newX, newY)) {
        setPosition(newX, newY);
    }
}

Direction Ghost::chooseNextDirection() {
    std::vector<Direction> validDirs = getValidDirections();
    if (validDirs.empty()) return lastDirection;

    if (scared) {
        // When scared, move randomly but try to avoid PacMan
        PacMan* pacman = gameBoard->getPacMan();
        if (pacman) {
            // Find direction that maximizes distance to PacMan
            Direction bestDir = validDirs[0];
            int maxDistance = -1;
            
            for (Direction dir : validDirs) {
                int newX = getX();
                int newY = getY();
                switch(dir) {
                    case Direction::UP: newY--; break;
                    case Direction::DOWN: newY++; break;
                    case Direction::LEFT: newX--; break;
                    case Direction::RIGHT: newX++; break;
                }
                
                int distance = calculateDistance(newX, newY, pacman->getX(), pacman->getY());
                if (distance > maxDistance) {
                    maxDistance = distance;
                    bestDir = dir;
                }
            }
            return bestDir;
        }
        
        // Fallback to random movement if no PacMan reference
        int randomIndex = rand() % validDirs.size();
        return validDirs[randomIndex];
    } else {
        // When not scared, try to move towards PacMan
        PacMan* pacman = gameBoard->getPacMan();
        if (pacman) {
            // Find direction that minimizes distance to PacMan
            Direction bestDir = validDirs[0];
            int minDistance = INT_MAX;
            
            for (Direction dir : validDirs) {
                int newX = getX();
                int newY = getY();
                switch(dir) {
                    case Direction::UP: newY--; break;
                    case Direction::DOWN: newY++; break;
                    case Direction::LEFT: newX--; break;
                    case Direction::RIGHT: newX++; break;
                }
                
                int distance = calculateDistance(newX, newY, pacman->getX(), pacman->getY());
                if (distance < minDistance) {
                    minDistance = distance;
                    bestDir = dir;
                }
            }
            return bestDir;
        }
        
        return validDirs[0];
    }
}

std::vector<Direction> Ghost::getValidDirections() {
    std::vector<Direction> valid;
    int x = getX();
    int y = getY();
    
    // Check each direction
    if (gameBoard->isWalkable(x, y-1)) valid.push_back(Direction::UP);
    if (gameBoard->isWalkable(x, y+1)) valid.push_back(Direction::DOWN);
    if (gameBoard->isWalkable(x-1, y)) valid.push_back(Direction::LEFT);
    if (gameBoard->isWalkable(x+1, y)) valid.push_back(Direction::RIGHT);
    
    // Remove opposite of last direction to prevent back-and-forth movement
    Direction opposite;
    switch(lastDirection) {
        case Direction::UP: opposite = Direction::DOWN; break;
        case Direction::DOWN: opposite = Direction::UP; break;
        case Direction::LEFT: opposite = Direction::RIGHT; break;
        case Direction::RIGHT: opposite = Direction::LEFT; break;
    }
    valid.erase(std::remove(valid.begin(), valid.end(), opposite), valid.end());
    
    return valid;
}

int Ghost::calculateDistance(int x1, int y1, int x2, int y2) {
    // Manhattan distance
    return std::abs(x2 - x1) + std::abs(y2 - y1);
}

void Ghost::update() {
    moveAutomatically();
}

void Ghost::draw() {
    char representation = scared ? 'S' : 'G';  // 'S' for scared ghost, 'G' for normal
    
    if (scared) {
        std::cout << "\033[1;34m" << representation << "\033[0m";  // Blue when scared
    } else {
        std::cout << "\033[1;31m" << representation << "\033[0m";  // Red normally
    }
}

void Ghost::returnToSpawn() {
    if (gameBoard) {
        gameBoard->respawnGhost(this);
    }
}
