#pragma once

// Basic entity class for all game characters
class Entity {
public:
    int x, y;   // Position on the game board
    int dx, dy;  // Directions (-1/0/1)
    Entity() : x(0), y(0) {}
    Entity(int startX, int startY) : x(startX), y(startY), dx(0), dy(-1) {}
    
    virtual ~Entity() = default;
};
