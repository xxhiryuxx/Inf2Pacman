
#pragma once

/**
 * Manages the player's score throughout the game
 * Handles score updates and display
 */
class Score {
public:
    Score();     // Initialize score to 0
    ~Score();

    // Score operations
    void increase(int points);     // Add points to current score
    int getScore() const;         // Get current score value
    void display();               // Display score on screen

private:
    int currentScore;            // Current score value
};    void increase(int points);     // Add points to current score
