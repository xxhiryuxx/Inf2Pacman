#pragma once

struct Entity {
    int x, y;
    Entity(int startX, int startY) : x(startX), y(startY) {}
    virtual ~Entity() = default;
};
