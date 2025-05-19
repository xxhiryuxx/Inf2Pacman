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
#include "Coin.h"
#include "Fruit.h"
#include "Ghost.h"
#include <iostream>

PacMan::PacMan(GameBoard* board, Score* scorePtr) 
    : gameBoard(board), score(scorePtr), lives(3), powered(false) {}

void PacMan::processInput(char input) {
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

bool PacMan::tryMove(int newX, int newY) {
    if (!gameBoard) return false;
    
    if (gameBoard->isWalkable(newX, newY)) {
        setPosition(newX, newY);
        return true;
    }
    return false;
}

void PacMan::checkCollectibles() {
    if (!gameBoard || !score) return;

    Element* element = gameBoard->getElement(getX(), getY());
    
    if (Coin* coin = dynamic_cast<Coin*>(element)) {
        collectCoin();
        gameBoard->setElement(getX(), getY(), nullptr);
        score->increase(coin->getPoints());
        delete coin;
    }
    else if (Fruit* fruit = dynamic_cast<Fruit*>(element)) {
        collectFruit();
        gameBoard->setElement(getX(), getY(), nullptr);
        score->increase(fruit->getPoints());
        delete fruit;
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

bool PacMan::checkGhostCollision() {
    if (!gameBoard) return false;
    
    for (Ghost* ghost : gameBoard->getGhosts()) {
        if (ghost->getX() == getX() && ghost->getY() == getY()) {
            if (powered) {
                // Eat ghost
                score->increase(200);
                return false;
            }
            return true;
        }
    }
    return false;
}

void PacMan::update() {
    // Check for collisions
    if (checkGhostCollision()) {
        lives--;
        // Reset position will be handled by game class
    }
}

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
