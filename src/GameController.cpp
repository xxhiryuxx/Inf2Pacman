#include "GameController.h"
#include <conio.h>  // For _getch() on Windows

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
