/**
 * @file GameCharacter.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "Element.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * Base class for all movable game characters (PacMan and Ghosts)
 * Handles basic movement and position management
 */
class GameCharacter : public Element {
public:
    GameCharacter();
    virtual ~GameCharacter();

    // Movement and position methods
    void moveDirection(Direction dir);  // Update character's direction and orientation
    void getPosition(int& outX, int& outY) const;  // Get current grid position
    void setPosition(int newX, int newY);  // Set new grid position
    virtual void update() = 0;  // Update character state each game tick
    
    // From Element
    virtual void draw() override = 0;  // Draw character representation
    virtual bool isWalkable() override;  // Characters can't be walked through

protected:
    Direction currentDirection;
};

#endif
