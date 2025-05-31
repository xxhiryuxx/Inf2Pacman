#pragma once
#include "Entity.h"

struct Player : public Entity {
    int score;
    Player(int startX, int startY) : Entity(startX, startY), score(0) {}
};
