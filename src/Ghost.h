// Ghost struct: represents a ghost enemy
#pragma once
#include "Entity.h"

struct Ghost : public Entity {
    Ghost(int startX, int startY) : Entity(startX, startY) {}
};
