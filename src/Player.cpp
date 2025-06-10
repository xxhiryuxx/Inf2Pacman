#include "Player.h"
#include "GameBoard.h"
#include "Constants.h"
#include "Ghost.h"
#include "Game.h"
#include <vector>

void Player::movePacman(GameBoard& board) {
    static double lastMoveTime = 0.0;
    if (GetTime() - lastMoveTime < 0.12) return; // Bewegungsgeschwindigkeit steuern
    lastMoveTime = GetTime();

    // Spielfeldgrenzen prüfen
    int inputDx = 0, inputDy = 0;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) inputDy = -1;
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) inputDy = 1;
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inputDx = -1;
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputDx = 1;

    int nx = x + inputDx;
    int ny = y + inputDy;

    if (nx < 0 || nx >= board.getWidth() || ny < 0 || ny >= board.getHeight()) return;
    if (board.field[ny][nx] == Cell::WALL) return;

    x = nx;
    y = ny;

    if (board.field[ny][nx] == Cell::COIN) {
        board.field[ny][nx] = Cell::EMPTY;
        score += 10;
        board.coinsLeft--;
    } else if (board.field[ny][nx] == Cell::FRUIT) {
        board.field[ny][nx] = Cell::EMPTY;
        score += 100;
        board.coinsLeft--;
    }
}

// Checks for collisions between Pacman and ghosts
bool Player::checkCollision(const std::vector<Ghost>& ghosts) const {
    for (const auto& g : ghosts) {
        if (g.x == x && g.y == y) {
            return true;
        }
    }
    return false;
}