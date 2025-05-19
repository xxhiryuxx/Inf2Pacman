/**
 * @file Ghost.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 15:35
 */

#pragma once
#include "GameCharacter.h"
class GameBoard; // Forward declaration

/**
 * Represents an enemy ghost that chases PacMan
 * Implements autonomous movement and collision detection
 */
class Ghost : public GameCharacter {
public:
    Ghost(GameBoard* board = nullptr);  // Create ghost with optional game board
    virtual ~Ghost() override;

    // Ghost behavior
    void moveAutomatically();  // Move ghost based on AI behavior
    bool checkPacManCollision();  // Check if ghost has caught PacMan
    void setGameBoard(GameBoard* board) { gameBoard = board; }  // Set game board reference

    // From GameCharacter
    virtual void update() override;  // Update ghost state each game tick
    virtual void draw() override;    // Draw ghost representation

private:
    // AI helper methods
    Direction chooseNextDirection();  // Decide next movement direction
    std::vector<Direction> getValidDirections();  // Get possible move directions
    int calculateDistance(int x1, int y1, int x2, int y2);  // Calculate Manhattan distance

    GameBoard* gameBoard;
    Direction lastDirection;
};
