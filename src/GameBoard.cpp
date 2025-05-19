#include "GameBoard.h"

GameBoard::GameBoard(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<Element*>(width, nullptr));
}

GameBoard::~GameBoard() {
    for (auto& row : grid) {
        for (auto& element : row) {
            delete element;
        }
    }
}

void GameBoard::draw() {
    // Will be implemented later
}

void GameBoard::setElement(int x, int y, Element* element) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        delete grid[y][x];
        grid[y][x] = element;
    }
}

Element* GameBoard::getElement(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return nullptr;
}

bool GameBoard::isWalkable(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height && grid[y][x]) {
        return grid[y][x]->isWalkable();
    }
    return false;
}

void GameBoard::updateRandomFruit() {
    // Will be implemented later
}
