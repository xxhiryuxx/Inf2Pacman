#ifndef PACMAN_H
#define PACMAN_H

#include "GameCharacter.h"

class PacMan : public GameCharacter {
public:
    PacMan();
    virtual ~PacMan();

    void handleInput(char input);
    void collectCoin();
    void collectFruit();
    bool checkGhostCollision();

    // From GameCharacter
    virtual void update() override;
    virtual void draw() override;
};

#endif // PACMAN_H
