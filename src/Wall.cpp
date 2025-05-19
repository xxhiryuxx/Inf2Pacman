#include "Wall.h"
#include <iostream>

Wall::Wall() {}

Wall::~Wall() {}

void Wall::draw() {
    std::cout << "#";
}

bool Wall::isWalkable() {
    return false;
}
