/**
 * @file Element.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once

/**
 * Base class for all game elements (characters, walls, collectibles)
 * Provides common functionality for positioning and rendering
 */
struct Position {
    int x;
    int y;
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class Element {
public:
    Element();  // Initialize element
    virtual ~Element();

    // Pure virtual methods
    virtual char draw() const = 0;  // Draw element on screen, returns character representation
    virtual bool isWalkable() const = 0;  // Can other elements move through this?

    // Position management
    void setPosition(int newX, int newY) { x = newX; y = newY; }  // Set grid position
    int getX() const { return x; }  // Get X coordinate
    int getY() const { return y; }  // Get Y coordinate

protected:
    // Grid coordinates
    int x;  // X position in grid
    int y;  // Y position in grid
};
