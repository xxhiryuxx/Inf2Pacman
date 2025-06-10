#pragma once
#include "Entity.h"
#include "GameBoard.h"
#include <vector>

class Player : public Entity {
public:
    // Player-specific attributes
    int score;
    Player(int startX, int startY) : Entity(startX, startY), score(0) {}
    GameBoard board;                // The game board (map, coins, walls, etc.)

    void movePacman(GameBoard& board);
    bool checkCollision(const std::vector<class Ghost>& ghosts) const;
};
