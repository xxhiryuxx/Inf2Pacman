/**
 * @file GameController.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "PacMan.h"
#include <map>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class GameController {
public:
    GameController();
    ~GameController();

    void setPacMan(PacMan* player);
    char getInput();
    void interpretInput();
    void forwardToPacMan();
    Direction getLastDirection() const;
    
private:
    PacMan* pacman;
    char currentInput;
    Direction lastDirection;
    std::map<char, Direction> inputMap;
    
    void initializeInputMap();
    Direction mapInputToDirection(char input);
};
