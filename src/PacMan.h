#pragma once
#include "GameCharacter.h"

// Forward declarations
class GameBoard;
class Score;

/**
 * Represents the player-controlled PacMan character
 * Handles movement, collectibles, and ghost interactions
 */
class PacMan : public GameCharacter {
public:
    // Constructor/Destructor
    PacMan(GameBoard* board, Score* scorePtr);  // Initialize with game board and score
    virtual ~PacMan();

    // Game actions
    void processInput(char input);  // Handle keyboard input for movement
    bool tryMove(int newX, int newY);  // Attempt to move to new position
    void checkCollectibles();  // Check and collect items at current position
    void collectCoin();  // Handle coin collection
    void collectFruit();  // Handle fruit collection
    void collectPowerPellet();  // Handle power pellet collection
    bool checkGhostCollision();  // Check for ghost encounters
    Position getPosition() const; // Add this

    // From GameCharacter
    virtual void update() override;  // Update game state
    virtual void draw() override;  // Draw PacMan character

private:
    GameBoard* gameBoard;  // Reference to game board
    Score* score;  // Reference to score tracker
    int lives;  // Remaining lives
    bool powered;  // Power pellet status
    int powerTimer{0};  // Timer for power pellet effect
    static const int POWER_DURATION = 200;  // Duration of power pellet effect (in game ticks)
};