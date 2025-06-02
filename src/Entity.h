#pragma once

struct Entity {
    int x, y;
    Entity() : x(0), y(0) {}
    Entity(int startX, int startY) : x(startX), y(startY) {}
    virtual ~Entity() = default;
};
