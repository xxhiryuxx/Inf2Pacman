#include "GameBoard.h"

GameBoard::GameBoard() : grid(HEIGHT, std::vector<GridPoint>(WIDTH)), pacman(nullptr) {
    // Initialize basic board with walls and coins
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH-1 || y == 0 || y == HEIGHT-1) {
                grid[y][x].setContent(CellContent::WALL);
            } else {
                grid[y][x].setContent(CellContent::COIN);
            }
        }
    }

    // Create PacMan at position (1,1)
    pacman = new PacMan(this, nullptr);
    pacman->setPosition(1, 1);

    // Create one ghost at opposite corner
    Ghost* ghost = new Ghost(this);
    ghost->setPosition(WIDTH-2, HEIGHT-2);
    ghosts.push_back(ghost);
}

GameBoard::~GameBoard() {
    delete pacman;
    for (auto ghost : ghosts) {
        delete ghost;
    }
}

void GameBoard::update() {
    if (pacman) {
        pacman->update();
    }
    for (auto ghost : ghosts) {
        ghost->update();
    }
}

GridPoint* GameBoard::getGridPoint(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return nullptr;
    }
    return &grid[y][x];
}

PacMan* GameBoard::getPacMan() const {
    return pacman;
}

std::vector<Ghost*> GameBoard::getGhosts() const {
    return ghosts;
}

int GameBoard::getRemainingCollectibles() const {
    int count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x].isCollectible()) {
                count++;
            }
        }
    }
    return count;
}