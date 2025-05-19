#include "PacMan.h"
#include <iostream>

PacMan::PacMan() : lives(3), powered(false) {}

PacMan::~PacMan() {}

void PacMan::processInput(char input) {
    switch(input) {
        case 'w':
            moveDirection(Direction::UP);
            break;
        case 's':
            moveDirection(Direction::DOWN);
            break;
        case 'a':
            moveDirection(Direction::LEFT);
            break;
        case 'd':
            moveDirection(Direction::RIGHT);
            break;
    }
}

void PacMan::collectCoin() {
    // Will be implemented later
}

void PacMan::collectFruit() {
    // Will be implemented later
}

bool PacMan::checkGhostCollision() {
    // Will be implemented later
    return false;
}

void PacMan::update() {
    // Will be implemented later
}

void PacMan::draw() {
    // Simple representation of PacMan based on direction
    char representation;
    switch(currentDirection) {
        case Direction::RIGHT:
            representation = '>';
            break;
        case Direction::LEFT:
            representation = '<';
            break;
        case Direction::UP:
            representation = '^';
            break;
        case Direction::DOWN:
            representation = 'v';
            break;
    }
    std::cout << representation;
}
