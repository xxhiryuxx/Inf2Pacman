/**
 * @file Ghost.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "Ghost.h"
#include <iostream>

Ghost::Ghost() {}

Ghost::~Ghost() {}

void Ghost::moveAutomatically() {
    // Will be implemented later - AI movement logic
}

void Ghost::chasePacMan() {
    // Will be implemented later - pathfinding towards PacMan
}

bool Ghost::checkPacManCollision() {
    // Will be implemented later
    return false;
}

void Ghost::update() {
    // Will be implemented later
}

void Ghost::draw() {
    std::cout << "G";  // Simple ghost representation
}
