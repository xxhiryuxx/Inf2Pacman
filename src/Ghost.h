#pragma once
#include "Entity.h"
#include "raylib.h"
#include "GameBoard.h"
#include "Player.h"

class Ghost : public Entity {
public:
    int startX, startY;
    Color color;
    int speed = 5;          // Frames zwischen Bewegungen (je kleiner, desto schneller)
    int moveCounter = 0;    // Zähler für Bewegungstiming

    Ghost(int startX, int startY, Color color);

    void resetToStart();
    void update(const GameBoard& board, const Player& pacman);
};
