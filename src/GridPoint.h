/**
 * @file GridPoint.h
 * @author Lorin Meub
 * @editor Lorin Meub
 * @date 19.05.2025
 * @time 14:02
 */

#pragma once
#include "Element.h"
#include "Coin.h"
#include "Fruit.h"

class GridPoint : public Element {
public:
    GridPoint();
    virtual ~GridPoint();

    void setWalkable(bool walkable);
    void setCoin(Coin* coin);
    void setFruit(Fruit* fruit);
    Coin* removeCoin();
    Fruit* removeFruit();
    bool hasCoin() const;
    bool hasFruit() const;

    // From Element
    virtual void draw() override;
    virtual bool isWalkable() override;

private:
    bool walkable;
    Coin* coin;    Fruit* fruit;
};
