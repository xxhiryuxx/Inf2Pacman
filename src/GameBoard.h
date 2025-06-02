
#pragma once
#include <vector>
#include "Constants.h"
#include "MazeCell.h"

class GameBoard {
public:
    std::vector<std::vector<Cell>> field;
    int coinsLeft;
    bool fruitPresent;
    int fruitX, fruitY;
    void generateRandomMap();
private:
    static void shuffleDirections(int dx[], int dy[], int n);
    static void dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze);
};
