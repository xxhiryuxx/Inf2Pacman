#include "GameBoard.h"
#include <iostream>
#include <vector>

const int WIDTH = 15;
const int HEIGHT = 15;

class GameBoard {
private:
    std::vector<std::vector<char>> board;

public:
    GameBoard() {
        board.resize(HEIGHT, std::vector<char>(WIDTH, '.'));

        // Ränder als Wände
        for (int i = 0; i < HEIGHT; ++i) {
            board[i][0] = '#';
            board[i][WIDTH - 1] = '#';
        }
        for (int j = 0; j < WIDTH; ++j) {
            board[0][j] = '#';
            board[HEIGHT - 1][j] = '#';
        }

        // Beispielhafte innere Wände
        for (int i = 3; i < 12; ++i) {
            board[5][i] = '#';
            board[i][7] = '#';
        }
    }

    void draw() const {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    }

    bool isWall(int x, int y) const {
        return board[y][x] == '#';
    }

    bool collectPoint(int x, int y) {
        if (board[y][x] == '.') {
            board[y][x] = ' ';
            return true;
        }
        return false;
    }
};

// Beispielnutzung
/*
int main() {
    GameBoard gb;
    gb.draw();
    return 0;
}
*/