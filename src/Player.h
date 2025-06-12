#pragma once
#include "Entity.h"
#include "GameBoard.h"
#include <vector>

class Player : public Entity {
public:
    int score;
    GameBoard board;    // The game board (map, coins, walls, etc.)
    
    Player(int startX, int startY) : Entity(startX, startY), score(0) {}
    void movePacman(GameBoard& board);
    bool checkCollision(const std::vector<class Ghost>& ghosts) const;
};
