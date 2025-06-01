// Player struct: represents the player character (Pacman)
#pragma once
#include "Entity.h"

struct Player : public Entity {
    int score; // Current score
    Player(int startX, int startY) : Entity(startX, startY), score(0) {}
};
