#include "Ghost.h"
#include "GameBoard.h"
#include "Player.h"
#include "Constants.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>

Ghost::Ghost(int startX, int startY, Color color)
    : Entity(startX, startY), startX(startX), startY(startY), color(color) {
    speed = 30;       // Höhere Frequenz = flüssigere Bewegung
    moveCounter = 0;
}

void Ghost::resetToStart() {
    x = startX;
    y = startY;
    dx = 0;
    dy = -1;
    moveCounter = 0;
}

void Ghost::update(const GameBoard& board, const Player& pacman) {
    moveCounter++;
    if (moveCounter < speed) return;
    moveCounter = 0;

    std::vector<std::pair<int, int>> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0} // Oben, Rechts, Unten, Links
    };

    int bestDx = 0, bestDy = 0;
    int minDist = INT_MAX;
    bool foundValidDirection = false;

    // 1. Prüfe alle Richtungen außer Umkehr
    for (auto [ndx, ndy] : directions) {
        if (-ndx == dx && -ndy == dy) continue; // Kein Reverse
        
        int nx = x + ndx;
        int ny = y + ndy;
        if (nx >= 0 && nx < board.getWidth() && 
            ny >= 0 && ny < board.getHeight() && 
            board.field[ny][nx] != WALL) {
            
            int dist = abs(pacman.x - nx) + abs(pacman.y - ny);
            if (dist < minDist || (dist == minDist && rand() % 2 == 0)) {
                minDist = dist;
                bestDx = ndx;
                bestDy = ndy;
                foundValidDirection = true;
            }
        }
    }

    // 2. Falls keine Richtung: Prüfe Reverse-Richtung
    if (!foundValidDirection) {
        int reverseDx = -dx;
        int reverseDy = -dy;
        int nx = x + reverseDx;
        int ny = y + reverseDy;
        if (nx >= 0 && nx < board.getWidth() && 
            ny >= 0 && ny < board.getHeight() && 
            board.field[ny][nx] != WALL) {
            bestDx = reverseDx;
            bestDy = reverseDy;
            foundValidDirection = true;
        }
    }

    // 3. Nur bewegen, wenn Richtung gültig
    if (foundValidDirection) {
        dx = bestDx;
        dy = bestDy;
        x += dx;
        y += dy;
    }
}
