#pragma once
#include "Element.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class GameCharacter : public Element {
public:
    GameCharacter();
    virtual ~GameCharacter();

    void moveDirection(Direction dir);
    void getPosition(int& outX, int& outY) const;
    void setPosition(int newX, int newY);
    virtual void update() = 0;
    
    // From Element
    virtual void draw() override = 0;
    virtual bool isWalkable() override;

protected:
    Direction currentDirection;
};

#endif // GAMECHARACTER_H
