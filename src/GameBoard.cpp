
#include "GameBoard.h"
#include "MazeCell.h"
#include <cstdlib>

void GameBoard::shuffleDirections(int dx[], int dy[], int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(dx[i], dx[j]);
        std::swap(dy[i], dy[j]);
    }
}

void GameBoard::dfs(int x, int y, std::vector<std::vector<MazeCell>>& maze) {
    maze[y][x].visited = true;
    maze[y][x].wall = false;
    int dx[4] = {2, -2, 0, 0};
    int dy[4] = {0, 0, 2, -2};
    shuffleDirections(dx, dy, 4);
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx > 0 && nx < (int)maze[0].size() - 1 &&
            ny > 0 && ny < (int)maze.size() - 1 &&
            !maze[ny][nx].visited) {
            maze[y + dy[i] / 2][x + dx[i] / 2].wall = false;
            dfs(nx, ny, maze);
        }
    }
}

void GameBoard::generateRandomMap() {
    const int mazeHeight = HEIGHT % 2 == 0 ? HEIGHT + 1 : HEIGHT;
    const int mazeWidth = WIDTH % 2 == 0 ? WIDTH + 1 : WIDTH;
    std::vector<std::vector<MazeCell>> maze(mazeHeight, std::vector<MazeCell>(mazeWidth));
    dfs(1, 1, maze);
    coinsLeft = 0;
    field.resize(HEIGHT, std::vector<Cell>(WIDTH, WALL));
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (y < mazeHeight && x < mazeWidth && !maze[y][x].wall) {
                field[y][x] = COIN;
                coinsLeft++;
            } else {
                field[y][x] = WALL;
            }
        }
    }
    int extraConnections = (WIDTH * HEIGHT) / 6;
    for (int i = 0; i < extraConnections; ++i) {
        int x = 1 + rand() % (WIDTH - 2);
        int y = 1 + rand() % (HEIGHT - 2);
        if (field[y][x] == WALL) {
            if (field[y][x - 1] != WALL && field[y][x + 1] != WALL) {
                field[y][x] = COIN;
                coinsLeft++;
            }
            else if (field[y - 1][x] != WALL && field[y + 1][x] != WALL) {
                field[y][x] = COIN;
                coinsLeft++;
            }
        }
    }
    fruitPresent = false;
    fruitX = fruitY = 0;
}
