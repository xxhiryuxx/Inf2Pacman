/**
 * @file Coin.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "Element.h"

class Coin : public Element {
public:
    Coin();
    virtual ~Coin();

    int getPoints() const;
    
    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;

private:    int points;
};
