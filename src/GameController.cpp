/**
 * @file GameController.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "GameController.h"
#include <conio.h>// For _getch() on Windows

GameController::GameController() : currentInput(0) {}

GameController::~GameController() {}

char GameController::getInput() {
    currentInput = _getch();
    return currentInput;
}

void GameController::interpretInput() {
    // Will be implemented later
}

void GameController::forwardToPacMan() {
    // Will be implemented later
}
