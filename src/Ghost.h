#pragma once
#include "Entity.h"
#include "raylib.h"

class Ghost : public Entity {
    public:
    int startX, startY;
    Color color;

    Ghost(int startX, int startY, Color color);

    void resetToStart();
};