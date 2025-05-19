/**
 * @file Element.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once

class Element {
public:
    Element();
    virtual ~Element();
    virtual void draw() = 0;
    virtual bool isWalkable() = 0;

    // Position management
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    int getX() const { return x; }
    int getY() const { return y; }

protected:
    // Common properties for all elements
    int x;
    int y;
};
