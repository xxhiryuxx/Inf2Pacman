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
    : GameCharacter(), gameBoard(board), score(scorePtr), lives(3), powered(false) {}

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

    if (tryMove(newX, newY)) {
        checkCollectibles();
    }
}

// Attempt to move PacMan to a new position
// Returns true if the move was successful, false if blocked
bool PacMan::tryMove(int newX, int newY) {
    if (!gameBoard) return false;
    
    if (gameBoard->isWalkable(newX, newY)) {
        setPosition(newX, newY);
        return true;
    }
    return false;
}

// Check and collect any coins or fruits at the current position
void PacMan::checkCollectibles() {
    if (!gameBoard || !score) return;

    GridPoint& currentCell = gameBoard->getGridPoint(getX(), getY());
    CellContent content = currentCell.getContent();
    
    if (content == CellContent::COIN) {
        collectCoin();
        currentCell.setContent(CellContent::EMPTY);
        score->increase(10);
    }
    else if (content == CellContent::FRUIT) {
        collectFruit();
        currentCell.setContent(CellContent::EMPTY);
        score->increase(100);
    }
}

void PacMan::collectCoin() {
    if (score) {
        score->increase(10);
    }
}

void PacMan::collectFruit() {
    if (score) {
        score->increase(100);
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
    // Check for collisions and handle life loss
    if (checkGhostCollision()) {
        lives--;
        // Reset position will be handled by game class
    }
}

// Draw PacMan's representation on the game board
void PacMan::draw() {
    // Simple representation of PacMan based on direction
    char representation;
    switch(currentDirection) {
        case Direction::RIGHT:
            representation = '>';
            break;        case Direction::LEFT:
            representation = '<';
            break;
        case Direction::UP:
            representation = '^';
            break;
        case Direction::DOWN:
            representation = 'v';
            break;
    }
    if (powered) {
        std::cout << "\033[1;33m" << representation << "\033[0m";  // Yellow when powered
    } else {
        std::cout << "\033[1;33m" << representation << "\033[0m";  // Yellow normally
    }
}
