/**
 * @file PacMan.cpp
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#include "PacMan.h"
#include "GameBoard.h"
#include "Score.h"
#include "Ghost.h"
#include <iostream>

// Initialize PacMan with game board and score tracking
PacMan::PacMan(GameBoard* board, Score* scorePtr)
    : GameCharacter(board->getStartPosition())
    , gameBoard(board)
    , score(scorePtr)
    , lives(3)
    , powered(false)
    , powerTimer(0)
{
}

PacMan::~PacMan() {
    // GameBoard and Score are owned by Game class
}

// Handle keyboard input for PacMan movement
void PacMan::processInput(char input) {
    // Get current position to calculate new position
    int newX = getX();
    int newY = getY();

    switch(input) {
        case 'w':
            moveDirection(Direction::UP);
            newY--;
            break;
        case 's':
            moveDirection(Direction::DOWN);
            newY++;
            break;
        case 'a':
            moveDirection(Direction::LEFT);
            newX--;
            break;
        case 'd':
            moveDirection(Direction::RIGHT);
            newX++;
            break;
        default:
            return;
    }

    tryMove(newX, newY);
}

// Attempt to move PacMan to a new position
// Returns true if the move was successful, false if blocked
bool PacMan::tryMove(int newX, int newY) {
    if (!gameBoard->isValidMove(newX, newY)) {
        return false;
    }

    position.x = newX;
    position.y = newY;
    return true;
}

// Check and collect any coins or fruits at the current position
void PacMan::checkCollectibles() {
    auto currentCell = gameBoard->getGridPoint(position);
    if (!currentCell) return;

    switch (currentCell->getContent()) {
        case CellContent::COIN:
            collectCoin();
            break;
        case CellContent::FRUIT:
            collectFruit();
            break;
        case CellContent::POWER_PELLET:
            collectPowerPellet();
            break;
        default:
            break;
    }
}

void PacMan::collectCoin() {
    score->addPoints(10);  // Standard coin value
    gameBoard->getGridPoint(position)->setContent(CellContent::EMPTY);
}

void PacMan::collectFruit() {
    score->addPoints(100);  // Fruit bonus
    gameBoard->getGridPoint(position)->setContent(CellContent::EMPTY);
}

void PacMan::collectPowerPellet() {
    powered = true;
    powerTimer = POWER_DURATION;
    score->addPoints(50);  // Power pellet bonus
    gameBoard->getGridPoint(position)->setContent(CellContent::EMPTY);
}

void PacMan::updatePowerTimer() {
    if (powerTimer > 0) {
        powerTimer--;
        if (powerTimer == 0) {
            powered = false;
        }
    }
}

// Check for collision with any ghost
// Returns true if PacMan should lose a life, false if ghost was eaten or no collision
bool PacMan::checkGhostCollision() {
    if (!gameBoard) return false;
    
    for (Ghost* ghost : gameBoard->getGhosts()) {
        if (ghost->getX() == getX() && ghost->getY() == getY()) {
            if (powered) {
                // Eat ghost when powered up
                score->increase(200);
                return false;
            }
            return true;  // Ghost collision when not powered
        }
    }
    return false;
}

// Update PacMan's state each game tick
void PacMan::update() {
    if (powered) {
        updatePowerTimer();
    }
    checkCollectibles();

    // Check for collisions and handle life loss
    if (checkGhostCollision()) {
        lives--;
        position = gameBoard->getStartPosition();
        powered = false;
        powerTimer = 0;
    }
}

// Draw PacMan's representation on the game board
void PacMan::draw() {
    // PacMan is represented by 'C' when normal, 'O' when powered
    return powered ? 'O' : 'C';
}

bool PacMan::isPowered() const {
    return powered;
}

int PacMan::getPowerTimer() const {
    return powerTimer;
}

int PacMan::getLives() const {
    return lives;
}

void PacMan::loseLife() {
    lives--;
    position = gameBoard->getStartPosition();
    powered = false;
    powerTimer = 0;
}

bool PacMan::isAlive() const {
    return lives > 0;
}

Position PacMan::getPosition() const {
    return position;
}
