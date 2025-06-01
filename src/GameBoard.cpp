
#include "GameBoard.h"
#include <cstdlib>

// Generates a simple random map: border is wall, inside is coins, some random walls
void GameBoard::generateRandomMap() {
    field.resize(HEIGHT, std::vector<Cell>(WIDTH, WALL));
    coinsLeft = 0;
    fruitPresent = false;
    fruitX = fruitY = 0;
    // Fill inner area with coins, border with walls
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            field[y][x] = COIN;
            ++coinsLeft;
        }
    }
    // Optionally, add some random walls
    for (int i = 0; i < (WIDTH * HEIGHT) / 10; ++i) {
        int rx = 1 + rand() % (WIDTH - 2);
        int ry = 1 + rand() % (HEIGHT - 2);
        field[ry][rx] = WALL;
        --coinsLeft;
    }
}
