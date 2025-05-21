#include "Wall.h"

Wall::Wall() {}

Wall::~Wall() {}


char Wall::draw() const {
    return '#';
}

bool Wall::isWalkable() const {
    return false;
}
