#pragma once

class Entity {
public:
    int x, y;
    int dx, dy;  // Bewegungsrichtung (-1/0/1)
    Entity() : x(0), y(0) {}
    Entity(int startX, int startY) : x(startX), y(startY), dx(0), dy(-1) {}
    virtual ~Entity() = default;
};
