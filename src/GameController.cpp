/**
 * @file GameController.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "GameController.h"
#include <conio.h>

GameController::GameController()
    : pacman(nullptr)
    , currentInput(0)
    , lastDirection(Direction::NONE)
{
    initializeInputMap();
}

GameController::~GameController() {
    // PacMan is owned by GameBoard, don't delete it here
}

void GameController::initializeInputMap() {
    inputMap['w'] = Direction::UP;
    inputMap['W'] = Direction::UP;
    inputMap['s'] = Direction::DOWN;
    inputMap['S'] = Direction::DOWN;
    inputMap['a'] = Direction::LEFT;
    inputMap['A'] = Direction::LEFT;
    inputMap['d'] = Direction::RIGHT;
    inputMap['D'] = Direction::RIGHT;
}

void GameController::setPacMan(PacMan* player) {
    pacman = player;
}

char GameController::getInput() {
    if (_kbhit()) {
        currentInput = _getch();
        return currentInput;
    }
    return 0;
}

void GameController::interpretInput() {
    Direction newDirection = mapInputToDirection(currentInput);
    if (newDirection != Direction::NONE) {
        lastDirection = newDirection;
    }
}

Direction GameController::mapInputToDirection(char input) {
    auto it = inputMap.find(input);
    if (it != inputMap.end()) {
        return it->second;
    }
    return Direction::NONE;
}

void GameController::forwardToPacMan() {
    if (!pacman || lastDirection == Direction::NONE) {
        return;
    }

    switch (lastDirection) {
        case Direction::UP:
            pacman->moveUp();
            break;
        case Direction::DOWN:
            pacman->moveDown();
            break;
        case Direction::LEFT:
            pacman->moveLeft();
            break;
        case Direction::RIGHT:
            pacman->moveRight();
            break;
        default:
            break;
    }
}

Direction GameController::getLastDirection() const {
    return lastDirection;
}
