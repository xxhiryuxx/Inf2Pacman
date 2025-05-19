#include "GameCharacter.h"

GameCharacter::GameCharacter() : currentDirection(Direction::RIGHT) {}

GameCharacter::~GameCharacter() {}

void GameCharacter::moveDirection(Direction dir) {
    currentDirection = dir;
}

void GameCharacter::getPosition(int& outX, int& outY) const {
    outX = x;
    outY = y;
}

void GameCharacter::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

bool GameCharacter::isWalkable() {
    return false;
}
