/**
 * @file Ghost.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 15:35
 */

#pragma once
#include "GameCharacter.h"
class GameBoard; // Forward declaration

class Ghost : public GameCharacter {
public:
    Ghost(GameBoard* board = nullptr);
    virtual ~Ghost() override;

    void moveAutomatically();
    bool checkPacManCollision();
    void setGameBoard(GameBoard* board) { gameBoard = board; }

    // From GameCharacter
    virtual void update() override;
    virtual void draw() override;

private:
    Direction chooseNextDirection();
    std::vector<Direction> getValidDirections();
    int calculateDistance(int x1, int y1, int x2, int y2);

    GameBoard* gameBoard;
    Direction lastDirection;
};
