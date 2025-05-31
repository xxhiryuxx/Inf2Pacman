
#pragma once
#include <vector>
#include "Constants.h"

class GameBoard {
public:
    std::vector<std::vector<Cell>> field;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;
    void generateRandomMap();
};
