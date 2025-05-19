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
