#include "Player.h"
#include "Entity.h"
#include "GameBoard.h"
#include "Constants.h"
#include "Ghost.h"
#include "Game.h"
#include <vector>

// Player constructor initializes Pacman's starting position and score
Player::Player(int startX, int startY, int score) 
    : Entity(startX, startY), score(score) {}

// Handle Pacman's movement based on keyboard input
void Player::movePacman(GameBoard& board) {
    static double lastMoveTime = 0.0;
    if (GetTime() - lastMoveTime < 0.13) return; // Handle frame rate limit (The higher the value, the slower Pacman moves)
    lastMoveTime = GetTime();

    // Handle keyboard input for movement (WASD or Arrow keys)
    int inputDx = 0, inputDy = 0;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) inputDy = -1;
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) inputDy = 1;
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inputDx = -1;
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputDx = 1;

    // Calculate new position based on input
    int nx = x + inputDx;
    int ny = y + inputDy;

    // Check if the new position is within bounds and not a wall
    if (nx < 0 || nx >= board.getWidth() || ny < 0 || ny >= board.getHeight()) return;
    if (board.field[ny][nx] == Cell::WALL) return;

    // Update Pacman's position
    x = nx;
    y = ny;

    // Collect coins or fruit at the new position and update score
    if (board.field[ny][nx] == Cell::COIN) {
        board.field[ny][nx] = Cell::EMPTY;
        score += 10;
        board.coinsLeft--;
    } else if (board.field[ny][nx] == Cell::FRUIT) {
        board.field[ny][nx] = Cell::EMPTY;
        score += 100;
    }
}

// Check for collisions between Pacman and ghosts
bool Player::checkCollision(const std::vector<Ghost>& ghosts) const {
    for (const auto& g : ghosts) {
        if (g.x == x && g.y == y) {
            return true;
        }
    }
    return false;
}