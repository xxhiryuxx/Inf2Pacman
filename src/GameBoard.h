
#pragma once
#include <vector>
#include "Constants.h"
#include "MazeCell.h"


// GameBoard.h
// Declares the GameBoard class for managing the maze, coins, and fruit logic.
class GameBoard {
public:
    std::vector<std::vector<Cell>> field; // The game board grid
    int coinsLeft;                        // Number of coins remaining
    bool fruitPresent;                    // Is a fruit currently present?
    int fruitX, fruitY;                   // Fruit position

    // Generates a random maze and fills the board with coins and walls
    void generateRandomMap();
private:
    // Shuffles direction arrays for maze generation
    static void shuffleDirections(int dx[], int dy[], int n);
    // Depth-first search for maze generation
    static void dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze);
};
