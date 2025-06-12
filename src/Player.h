#pragma once
#include "Entity.h"
#include "GameBoard.h"
#include <vector>

class Player : public Entity {
private:
    int score;              // Player's score
    GameBoard board;        // Reference to the game board for collision checks

public:
    // Constructor to initialize player with starting position and score
    Player(int startX, int startY, int score);
    // Function to move the player based on input and update score
    void movePacman(GameBoard& board);
    // Function to check for collisions with ghosts
    bool checkCollision(const std::vector<class Ghost>& ghosts) const;
    // getter function for the player's score
    int getScore() const { return score; };
};