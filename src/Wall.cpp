/**
 * @file Ghost.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 17:56
 */

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
