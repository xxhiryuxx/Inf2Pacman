#pragma once
#include "GameCharacter.h"
#include <vector>

// Forward declarations
class GameBoard;
class PacMan;

/**
 * Represents an enemy ghost that chases PacMan
 * Implements autonomous movement and collision detection
 */
class Ghost : public GameCharacter {
public:
    Ghost(GameBoard* board = nullptr);  // Create ghost with optional game board
    ~Ghost() override;  // Virtual destructor inherited from GameCharacter    // Ghost behavior
    void moveAutomatically();                                   // Move ghost based on AI behavior
    bool checkPacManCollision();                               // Check if ghost has caught PacMan
    void setGameBoard(GameBoard* board) { gameBoard = board; } // Set game board reference
    void setScared(bool isScared) { scared = isScared; }      // Set ghost state when PacMan is powered
    bool isScared() const { return scared; }                   // Check if ghost is in scared state
    int getPointValue() const { return 200; }                  // Eating a ghost gives 200 points
    void returnToSpawn();                                      // Return ghost to spawn position
    Position getPosition() const { return Position{getX(), getY()}; } // Get ghost's current position

    // From GameCharacter
    virtual void update() override;  // Update ghost state each game tick
    virtual char draw() const override;    // Draw ghost representation

private:
    // AI helper methods
    Direction chooseNextDirection();  // Decide next movement direction
    std::vector<Direction> getValidDirections();  // Get possible move directions
    int calculateDistance(int x1, int y1, int x2, int y2);  // Calculate Manhattan distance

    // Member variables
    GameBoard* gameBoard;      // Reference to game board
    Direction lastDirection;   // Last movement direction
    bool scared{false};       // Ghost state when PacMan is powered
};
