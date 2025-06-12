#pragma once
#include "Entity.h"
#include "raylib.h"
#include "GameBoard.h"
#include "Player.h"

class Ghost : public Entity {
private:
    int startX, startY;     // Starting position
    Color color;            // Ghost color for rendering
    int speed;              // Frames between movements
    int moveCounter;        // Counter for movement frames

public:
    // Constructor to initialize ghost with starting position and color
    Ghost(int startX, int startY, Color color);
    // Function to move the ghost towards Pacman
    void update(const GameBoard& board, const Player& pacman);
    
    //void resetToStart();   // Function to reset ghost to starting position and direction, but not implemented 
};
