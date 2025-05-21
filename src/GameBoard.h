#pragma once

#include <vector>

class GameBoard {
private:
    std::vector<std::vector<char>> board;

public:
    GameBoard();
    void draw() const;
    bool isWall(int x, int y) const;
    bool collectPoint(int x, int y);

    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }
};