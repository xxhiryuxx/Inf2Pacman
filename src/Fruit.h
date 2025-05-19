/**
 * @file Fruit.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "Element.h"

class Fruit : public Element {
public:
    Fruit();
    virtual ~Fruit();

    int getPoints() const;
    
    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;

private:    int points;
};
