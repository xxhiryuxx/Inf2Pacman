#pragma once
#include "GameCharacter.h"

class Ghost : public GameCharacter {
public:
    Ghost();
    virtual ~Ghost();

    void moveAutomatically();
    void chasePacMan();
    bool checkPacManCollision();

    // From GameCharacter
    virtual void update() override;
    virtual void draw() override;
};

#endif // GHOST_H
