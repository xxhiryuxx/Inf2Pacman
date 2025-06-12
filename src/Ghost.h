#pragma once
#include "Entity.h"
#include "raylib.h"
#include "GameBoard.h"
#include "Player.h"

class Ghost : public Entity {
public:
    int startX, startY;     // Starting position
    Color color;            // Ghost color for rendering
    int speed = 0;          // Frames between movements
    int moveCounter = 0;    // Counter for movement frames

    Ghost(int startX, int startY, Color color);

    //void resetToStart();   // Function to reset ghost to starting position and direction, but not implemented 
    void update(const GameBoard& board, const Player& pacman);
};
