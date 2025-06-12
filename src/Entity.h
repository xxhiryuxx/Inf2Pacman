#pragma once

// Basic entity class for all game characters
class Entity {
public:
    int x, y;       // Position on the game board
    int dx, dy;     // Directions (-1/0/1)

    // Default constructor initializes position to (0, 0) and direction to (0, -1)
    Entity() : x(0), y(0) {}
    // Constructor to initialize position and direction
    Entity(int startX, int startY) : x(startX), y(startY), dx(0), dy(-1) {}
    
    // Virtual destructor for proper cleanup in derived classes
    virtual ~Entity() = default;
};
